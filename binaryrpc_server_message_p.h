#ifndef BINARYRPC_SERVER_MESSAGE_P_H
#define BINARYRPC_SERVER_MESSAGE_P_H

#include "binaryrpc/binaryrpc_server_message.h"

namespace binaryrpc {

class ServerMessagePrivate : public QObject{
  Q_OBJECT
public:

  ServerMessagePrivate(const QString &, const QVariantList &);

  friend class ServerMessage;

  int id;
  QString method_name;
  QVariantList parameters;

signals:

  void done(int id, const QVariant &return_value);

  void fault(int id, int fault_code, const QString &fault_string);
};

} //namespace binaryrpc

#endif // BINARYRPC_SERVER_MESSAGE_P_H
