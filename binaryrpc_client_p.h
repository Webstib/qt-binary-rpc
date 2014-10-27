#ifndef BINARYRPC_CLIENTP_H
#define BINARYRPC_CLIENTP_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include <binaryrpc/binaryrpc_client.h>
#include <binaryrpc/binaryrpc_response.h>
#include <binaryrpc/binaryrpc_request.h>

namespace binaryrpc {

class ClientPrivateData : public QSharedData {
public:
  ClientPrivateData(QObject *parent, bool threaded);

  ~ClientPrivateData();

  int id;

  QList<ClientMessage *> requests;

  static const int sock_oper_timeout = 5000;

  QTcpSocket *socket;

  QHostAddress peer_address;
  QHostAddress local_address;
  quint16 peer_port;
  quint16 local_port;

  QThread *thread;
};

class ClientPrivate : public QObject {
  Q_OBJECT
public:

  ClientPrivate(Client *parent, bool threaded);

  int genId();

signals:

  void sigOpen(const QHostAddress &host, quint16 port);

  void sigClose();

  void sigSend(binaryrpc::ClientMessage *message);

public slots:

  void open(const QHostAddress &host_name, quint16 port);

  void close();

  void errorSocket(QAbstractSocket::SocketError error_code);

  void decodeResponse();

  void send(binaryrpc::ClientMessage *message);

private:

  void procError(int return_code, const QString &error_message);

  ClientMessage *getMessage(int id);

  void faultAll(int code, const QString &reason);

  void done(int, const QVariant &);

  void fault(int, int, const QString &);

  QSharedDataPointer<ClientPrivateData> d;

  friend class Client;

  Client *parent;
};

} //namespace binaryrpc

#endif //BINARYRPC_CLIENTP_H
