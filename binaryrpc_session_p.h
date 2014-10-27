#ifndef BINARYRPC_SESSION_P_H
#define BINARYRPC_SESSION_P_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include "binaryrpc/binaryrpc_session.h"

namespace binaryrpc {

class SessionPrivateData : public QSharedData {
public:
  SessionPrivateData(QObject *parent, int sock_descriptor, bool threaded);

  ~SessionPrivateData();

  static const int sock_oper_timeout = 5000;

  int sock_descriptor;
  QTcpSocket *socket;

  QHostAddress peer_address;
  QHostAddress local_address;
  quint16 peer_port;
  quint16 local_port;

  QThread *thread;
};

class SessionPrivate : public QObject {
  Q_OBJECT
public:
  SessionPrivate(int sock_descriptor, Session *parent, bool threaded = true);

  ~SessionPrivate();

signals:

  void sigClose();

private slots:

  void socketConnected();

  void processRequest();

  void socketError(QAbstractSocket::SocketError);

  void close();

  void createMessage(int id,
                     const QString &method_name,
                     const QVariantList &parameters);

  void done(int id, const QVariant &return_value);

  void fault(int id, int fault_code, const QString &fault_string);

  void sendResponse(const QByteArray &data);

private:

  void procError(const QString &error_message);

  QSharedDataPointer<SessionPrivateData> d;

  friend class Session;

  Session *parent;
};

} //namespace binaryrpc

#endif // BINARYRPC_SESSION_P_H
