#ifndef BINARYRPC_ISERVER_H
#define BINARYRPC_ISERVER_H

#include <QtNetwork/QHostAddress>
#include "binaryrpc/binaryrpc_session.h"

namespace binaryrpc {

class BINARYRPC_EXPORT Server : public QObject {
  Q_OBJECT
public:
  Server();

  ~Server();

  QString getAddress();

  quint16 getPort() const;

  bool listen(const QHostAddress &host, quint16 port = 0);

  bool isListening() const;

  void close();

  QString errorString() const;

  QAbstractSocket::SocketError serverError() const;

signals:

  void newSession(binaryrpc::Session *session);

private:

  friend class ServerPrivate;

  ServerPrivate *p;
};

} //namespace binaryrpc

#endif //BINARYRPC_ISERVER_H
