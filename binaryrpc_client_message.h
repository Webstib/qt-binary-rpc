#ifndef BINARYRPC_MESSAGE_H
#define BINARYRPC_MESSAGE_H

#include <QtCore/QVariantList>
#include "binaryrpc/binaryrpc_global.h"

namespace binaryrpc {

class Client;
class ClientPrivate;
class ClientMessagePrivate;

class BINARYRPC_EXPORT ClientMessage : public QObject {
  Q_OBJECT
public:
  ClientMessage(const QString &method_name,
                const QVariantList &parameters = QVariantList());

  ~ClientMessage();

  QString name() const;

  QVariantList parameters() const;

signals:

  void done(binaryrpc::ClientMessage *msg, const QVariant &return_value);

  void fault(binaryrpc::ClientMessage *msg,
             int fault_code,
             const QString &fault_string);

private:

  friend class Client;

  friend class ClientPrivate;

  ClientMessagePrivate *d;
};

} //namespace binaryrpc

#endif // BINARYRPC_MESSAGE_H
