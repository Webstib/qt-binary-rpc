#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>
#include "binaryrpc/binaryrpc_session_p.h"
#include "binaryrpc/binaryrpc_response.h"
#include "binaryrpc/binaryrpc_request.h"
#include "binaryrpc/binaryrpc_server_message_p.h"
#include "binaryrpc/binaryrpc_encoder.h"
#include "binaryrpc/binaryrpc_decoder.h"

namespace binaryrpc {

SessionPrivateData::SessionPrivateData(QObject *parent,
                                       int sock_descriptor,
                                       bool threaded) :
  sock_descriptor(sock_descriptor),
  socket(new QTcpSocket(parent)),
  peer_port(0),
  local_port(0),
  thread(0) {
  if(QMetaType::type("QHostAddress") == 0)
    qRegisterMetaType<QHostAddress>("QHostAddress");
  if(QMetaType::type("QAbstractSocket::SocketError") == 0)
    qRegisterMetaType<QAbstractSocket::SocketError>(
      "QAbstractSocket::SocketError");

  if(threaded) {
    thread = new QThread(parent);
    parent->moveToThread(thread);
  }
}

SessionPrivateData::~SessionPrivateData() {
  if(socket) {
    socket->deleteLater();
    socket = 0;
  }
  if(thread) {
    delete thread;
    thread = 0;
  }
}

SessionPrivate::SessionPrivate(int sock_descriptor,
                               Session *parent,
                               bool threaded) :
  QObject(0),
  d(new SessionPrivateData(this, sock_descriptor, threaded)),
  parent(parent) {

  connect(this, SIGNAL(sigClose()), this, SLOT(close()));

  connect(d->socket, SIGNAL(readyRead()), this, SLOT(processRequest()));
  connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(socketError(QAbstractSocket::SocketError)));

  QTimer::singleShot(0, this, SLOT(socketConnected()));
}

SessionPrivate::~SessionPrivate() {
  parent = 0;
}

void SessionPrivate::socketConnected() {
  if(!d->socket->setSocketDescriptor(d->sock_descriptor)) {
    qFatal("Not set socket descriptor to binaryrpc::SessionPrivate");
  }

  d->peer_address = d->socket->peerAddress();
  d->local_address = d->socket->localAddress();
  d->peer_port = d->socket->peerPort();
  d->local_port = d->socket->localPort();

  emit parent->onOpen();
}

void SessionPrivate::processRequest() {
  while(d->socket->bytesAvailable() != 0) {
    if(d->socket->bytesAvailable() < Header::HEADER_LENGTH) {
      if(!d->socket->waitForReadyRead(SessionPrivateData::sock_oper_timeout)) {
        return procError("Not enough bytes for decode header");
      }
    }

    BinaryDecoder decoder(d->socket->read(Header::HEADER_LENGTH));

    Request request;

    decoder.decode((Header &)request);

    int need_read(request.getLength() - Header::HEADER_LENGTH);

    if(need_read > 0){
      if(d->socket->bytesAvailable() < need_read) {
        if(!d->socket->waitForReadyRead(SessionPrivateData::sock_oper_timeout)) {
          return procError("Not enough bytes for decode body");
        }
      }
    } else {
      return procError("Packet length is null");
    }

    decoder.addBody(d->socket->read(need_read));

    if(decoder.decode(request)) {
      createMessage(request.getId(),
                    request.getMethodName(),
                    request.getParameters());
    } else {
      return procError("Failed decode body");
    }
  }
}

void SessionPrivate::createMessage(int id,
                                   const QString &method_name,
                                   const QVariantList &parameters) {
  ServerMessage *msg(new ServerMessage(method_name, parameters));

  msg->d->id = id;

  connect(msg->d, SIGNAL(fault(int, int, const QString &)),
          this, SLOT(fault(int, int, const QString &)));
  connect(msg->d, SIGNAL(done(int, const QVariant &)),
          this, SLOT(done(int, const QVariant &)));

  emit parent->newMessage(msg);
}

void SessionPrivate::socketError(QAbstractSocket::SocketError) {
  procError(d->socket->errorString());
}

void SessionPrivate::close() {
  if(d->socket->state() == QAbstractSocket::ConnectedState) {
    d->socket->disconnectFromHost();

    if(d->socket->state() == QAbstractSocket::UnconnectedState ||
       d->socket->waitForDisconnected(SessionPrivateData::sock_oper_timeout)) {
      emit parent->onClose();
    } else {
      procError(QString("Timeout disconnect from %1:%2")
                  .arg(d->peer_address.toString())
                  .arg(d->peer_port));
    }
  }
}

void SessionPrivate::done(int id, const QVariant &return_value) {
  Response response(id, return_value);

  response.calcLength();

  BinaryEncoder encoder;
  encoder << response;

  sendResponse(encoder.getBuff());
}

void SessionPrivate::fault(int id, int fault_code, const QString &fault_string) {
  Response response(id, fault_code, fault_string);

  response.calcLength();

  BinaryEncoder encoder;
  encoder << response;

  sendResponse(encoder.getBuff());
}

void SessionPrivate::sendResponse(const QByteArray &bytes) {
  if(d->socket->state() == QAbstractSocket::ConnectedState &&
     d->socket->isValid()) {

    qint64 write_bytes(d->socket->write(bytes));

    d->socket->flush();

    if(write_bytes < bytes.size()) {
      d->socket->waitForBytesWritten(SessionPrivateData::sock_oper_timeout);
    }
  }
}

void SessionPrivate::procError(const QString &error_message) {
  if(d->socket->state() == QAbstractSocket::ConnectedState) {
    d->socket->abort();
  }

  if(d->socket->error() == QAbstractSocket::RemoteHostClosedError) {
    emit parent->onClose();
  } else {
    emit parent->onError(error_message);
  }
}

Session::Session(int sock_descriptor, bool threaded) :
  p(new SessionPrivate(sock_descriptor, this, threaded)) {
  if(threaded) {
    QTimer::singleShot(0, this, SLOT(startProcess()));
  }
}

Session::~Session() {
  if(p) {
    if(p->d->thread) {
      if(p->d->thread->isRunning()) {
        p->d->thread->quit();
        p->d->thread->wait();
      }
    }
    delete p;
    p = 0;
  }
}

QHostAddress Session::peerAddress() const {
  return p->d->peer_address;
}

quint16 Session::peerPort() const {
  return p->d->peer_port;
}

QHostAddress Session::localAddress() const {
  return p->d->local_address;
}

quint16 Session::localPort() const {
  return p->d->local_port;
}

void Session::close() {
  emit p->sigClose();
}

void Session::startProcess() {
  if(p->d->thread && !p->d->thread->isRunning()) {
    p->d->thread->start();
  }
}

} //namespace binaryrcp
