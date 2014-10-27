#include "binaryrpc/binaryrpc_server_message_p.h"

namespace binaryrpc {

ServerMessagePrivate::ServerMessagePrivate(const QString &method_name,
                                           const QVariantList &parameters)
  : id(-1), method_name(method_name), parameters(parameters) {

}

ServerMessage::ServerMessage(const QString &method_name,
                             const QVariantList &parameters)
  : d(new ServerMessagePrivate(method_name, parameters)) {

}

ServerMessage::~ServerMessage() {
  if(d) {
    delete d;
    d = 0;
  }
}

QString ServerMessage::name() const {
  return d->method_name;
}

QVariantList ServerMessage::parameters() const {
  return d->parameters;
}

void ServerMessage::done(const QVariant &return_value) {
  emit d->done(d->id, return_value);
}

void ServerMessage::fault(int fault_code,
                          const QString &fault_string) {
  emit d->fault(d->id, fault_code, fault_string);
}

} //namespace binaryrpc

