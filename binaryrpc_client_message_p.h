#ifndef BINARYRPC_MESSAGE_P_H
#define BINARYRPC_MESSAGE_P_H

#include "binaryrpc/binaryrpc_client_message.h"

namespace binaryrpc {

class ClientMessagePrivate {
public:
  ClientMessagePrivate(const QString &, const QVariantList &);
  int id;
  QString method_name;
  QVariantList parameters;
};

} //namespace binaryrpc

#endif // BINARYRPC_MESSAGE_P_H
