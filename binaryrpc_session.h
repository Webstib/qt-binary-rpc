#ifndef BINARYRPC_ISESSION_H
#define BINARYRPC_ISESSION_H

#include <QtNetwork/QHostAddress>
#include "binaryrpc/binaryrpc_server_message.h"

namespace binaryrpc {

class SessionPrivate;
class ServerPrivate;

class BINARYRPC_EXPORT Session : public QObject {
  Q_OBJECT
public:

  Session(int sock_descriptor, bool threaded = true);

  ~Session();

  QHostAddress peerAddress() const;

  quint16 peerPort() const;

  QHostAddress localAddress() const;

  quint16 localPort() const;

public slots:

  void close();

private slots:

  void startProcess();

signals:

  void onOpen();

  void onClose();

  void onError(const QString &error_message);

  void newMessage(binaryrpc::ServerMessage *message);

private:

  friend class SessionPrivate;

  SessionPrivate *p;
};

} //namespace binaryrpc

#endif //BINARYRPC_ISESSION_H
