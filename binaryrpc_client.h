#ifndef BINARYRPC_CLIENT_H
#define BINARYRPC_CLIENT_H

#include <QtNetwork/QHostAddress>
#include "binaryrpc/binaryrpc_client_message.h"

namespace binaryrpc {

class ClientPrivate;

class BINARYRPC_EXPORT Client : public QObject {
  Q_OBJECT
public:

  Client(bool threaded = true);

  ~Client();

  QHostAddress peerAddress() const;

  quint16 peerPort() const;

  QHostAddress localAddress() const;

  quint16 localPort() const;

signals:

  void onOpen();

  void onClose();

  void onError(const QString &);

public slots:

  void open(const QHostAddress &host, quint16 port);

  void close();

  void send(binaryrpc::ClientMessage *message);

private slots:

  void startProcess();

private:

  friend class ClientPrivate;

  ClientPrivate *p;
};

} //namespace binaryrpc

#endif //BINARYRPC_CLIENT_H
