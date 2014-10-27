#ifndef BINARYRPC_SERVER_P_H
#define BINARYRPC_SERVER_P_H

#include <QtNetwork/QTcpServer>
#include <QtCore/QReadWriteLock>
#include "binaryrpc/binaryrpc_server.h"

namespace binaryrpc {

class Server;

class ServerPrivate : public QTcpServer {
  Q_OBJECT
public:

  ServerPrivate(Server *parent);

  ~ServerPrivate();

protected:

  void incomingConnection(int socket_descriptor);

private:

  Server *parent;
};

} //namespace binaryrpc

#endif // BINARYRPC_SERVER_P_H
