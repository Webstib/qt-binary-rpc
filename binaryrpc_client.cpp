#include <QtCore/QTimer>
#include "binaryrpc/binaryrpc_client_p.h"
#include "binaryrpc/binaryrpc_client_message_p.h"
#include "binaryrpc/binaryrpc_encoder.h"
#include "binaryrpc/binaryrpc_decoder.h"

namespace binaryrpc {

ClientPrivateData::ClientPrivateData(QObject *parent, bool threaded) :
  id(-1),
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

ClientPrivateData::~ClientPrivateData() {
  if(socket) {
    socket->deleteLater();
    socket = 0;
  }
  if(thread) {
    delete thread;
    thread = 0;
  }
}

ClientPrivate::ClientPrivate(Client *parent, bool threaded) :
  QObject(0),
  d(new ClientPrivateData(this, threaded)),
  parent(parent) {
  connect(this, SIGNAL(sigOpen(const QHostAddress &, quint16)),
          this, SLOT(open(const QHostAddress &, quint16)));
  connect(this, SIGNAL(sigClose()), this, SLOT(close()));
  connect(this, SIGNAL(sigSend(binaryrpc::ClientMessage *)),
          this, SLOT(send(binaryrpc::ClientMessage *)));

  connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(errorSocket(QAbstractSocket::SocketError)));
  connect(d->socket, SIGNAL(readyRead()), this, SLOT(decodeResponse()));
}

void ClientPrivate::open(const QHostAddress &host_name, quint16 port) {
  if(d->socket->state() == QAbstractSocket::UnconnectedState) {
    d->peer_address = host_name;
    d->peer_port = port;
    d->socket->connectToHost(host_name, port);
    if(d->socket->waitForConnected(ClientPrivateData::sock_oper_timeout)) {
      d->local_address = d->socket->localAddress();
      d->local_port = d->socket->localPort();
      emit parent->onOpen();
    } else {
      delete d->socket;
      d->socket = new QTcpSocket(parent);
      connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)),
              this, SLOT(errorSocket(QAbstractSocket::SocketError)));
      connect(d->socket, SIGNAL(readyRead()), this, SLOT(decodeResponse()));
      emit parent->onError(QString("Timeout connect from %1:%2")
                             .arg(host_name.toString())
                             .arg(port));
    }
  }
}

void ClientPrivate::close() {
  if(d->socket->state() == QAbstractSocket::ConnectedState) {
    d->socket->disconnectFromHost();
    if(d->socket->state() != QAbstractSocket::UnconnectedState &&
       d->socket->waitForDisconnected(ClientPrivateData::sock_oper_timeout)) {
      d->socket->abort();
    }

    emit parent->onClose();
  }
}

void ClientPrivate::errorSocket(QAbstractSocket::SocketError code) {
  if(code != QAbstractSocket::ConnectionRefusedError) {
    procError(BRPC_ERR_CODE_SOCK, d->socket->errorString());
  }
}

void ClientPrivate::decodeResponse() {
  while(d->socket->bytesAvailable() != 0) {
    if(d->socket->bytesAvailable() < Header::HEADER_LENGTH) {
      if(!d->socket->waitForReadyRead(ClientPrivateData::sock_oper_timeout)) {
        return procError(BRPC_ERR_CODE_DECODE,
                         "Not enough bytes for decode header");
      }
    }

    BinaryDecoder decoder(d->socket->read(Header::HEADER_LENGTH));

    Response response;

    decoder.decode((Header &)response);

    int need_read(response.getLength() - Header::HEADER_LENGTH);

    if(need_read > 0){
      if(d->socket->bytesAvailable() < need_read) {
        if(!d->socket->waitForReadyRead(ClientPrivateData::sock_oper_timeout)) {
          return procError(BRPC_ERR_CODE_DECODE,
                           "Not enough bytes for decode body");
        }
      }
    } else {
      return procError(BRPC_ERR_CODE_DECODE, "Packet length is null");
    }

    decoder.addBody(d->socket->read(need_read));

    if(decoder.decode(response)) {
      if(response.isFault()) {
        fault(response.getId(),
              response.getFaultCode(),
              response.getFaultString());
      } else {
        done(response.getId(), response.getReturnValue());
      }
    } else {
      procError(BRPC_ERR_CODE_DECODE, "Failed decode body");
    }
  }
}

void ClientPrivate::send(binaryrpc::ClientMessage *message) {
  if(message == 0) {
    return;
  }

  if(d->requests.contains(message)) {
    return;
  }

  message->d->id = genId();

  Request request(message->d->id,
                  message->d->method_name,
                  message->d->parameters);

  BinaryEncoder encoder;

  request.calcLength();

  encoder << request;

  if(d->socket->state() == QAbstractSocket::ConnectedState &&
     d->socket->isValid()) {

    QByteArray bytes(encoder.getBuff());

    qint64 write_bytes(d->socket->write(bytes));

    d->socket->flush();

    if(write_bytes < bytes.size()) {
      if(d->socket->waitForBytesWritten(ClientPrivateData::sock_oper_timeout)) {
        d->requests.push_back(message);
      } else {
        emit message->fault(message, BRPC_ERR_CODE_SOCK, "Not send to socket");
      }
    } else {
      d->requests.push_back(message);
    }
  } else {
    emit message->fault(message, BRPC_ERR_CODE_SOCK, "Not send to socket");
  }
}

void ClientPrivate::procError(int return_code, const QString &error_message) {
  emit parent->onError(error_message);

  faultAll(return_code, error_message);

  if(d->socket->state() == QAbstractSocket::ConnectedState) {
    d->socket->abort();
  }
}

ClientMessage *ClientPrivate::getMessage(int id) {
  foreach(ClientMessage *m, d->requests) {
    if(m->d->id == id) {
      return m;
    }
  }

  return 0;
}

void ClientPrivate::faultAll(int code, const QString &reason) {
  foreach(ClientMessage *m, d->requests) {
    emit m->fault(m, code, reason);
  }

  d->requests.clear();
}

void ClientPrivate::done(int id, const QVariant &return_value) {
  ClientMessage *msg(getMessage(id));

  if(msg == 0) {
    return;
  }

  d->requests.removeAll(msg);

  emit msg->done(msg, return_value);
}

void ClientPrivate::fault(int id, int code, const QString &str) {
  ClientMessage *msg(getMessage(id));

  if(msg == 0) {
    return;
  }

  d->requests.removeAll(msg);

  emit msg->fault(msg, code, str);
}

int ClientPrivate::genId() {
  if(d->id == 0x7FFFFFFF) {
    d->id = 0;
  }

  return d->id++;
}

Client::Client(bool threaded) : p(new ClientPrivate(this, threaded)) {
  if(threaded) {
    QTimer::singleShot(0, this, SLOT(startProcess()));
  }
}

Client::~Client() {
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

QHostAddress Client::peerAddress() const {
  return p->d->peer_address;
}

quint16 Client::peerPort() const {
  return p->d->peer_port;
}

QHostAddress Client::localAddress() const {
  return p->d->local_address;
}

quint16 Client::localPort() const {
  return p->d->local_port;
}

void Client::open(const QHostAddress &host, quint16 port) {
  emit p->sigOpen(host, port);
}

void Client::close() {
  emit p->sigClose();
}

void Client::send(ClientMessage *msg) {
  emit p->sigSend(msg);
}

void Client::startProcess() {
  if(p->d->thread && !p->d->thread->isRunning()) {
    p->d->thread->start();
  }
}

} //namespace binaryrpc
