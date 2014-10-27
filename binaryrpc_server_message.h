#ifndef BINARYRPC_SERVER_MESSAGE_H
#define BINARYRPC_SERVER_MESSAGE_H

#include <QtCore/QVariantList>
#include "binaryrpc/binaryrpc_global.h"

namespace binaryrpc {

class SessionPrivate;
class ServerMessagePrivate;

class BINARYRPC_EXPORT ServerMessage : public QObject {
  Q_OBJECT
public:
  ServerMessage(const QString &method_name,
                const QVariantList &parameters = QVariantList());

  ~ServerMessage();

  QString name() const;

  QVariantList parameters() const;

public slots:

  void done(const QVariant &return_value);

  void fault(int fault_code, const QString &fault_string);

private:

  friend class SessionPrivate;

  ServerMessagePrivate *d;
};

} //namespace binaryrpc

#endif // BINARYRPC_SERVER_MESSAGE_H
