#include "binaryrpc/binaryrpc_client_message_p.h"

namespace binaryrpc {

ClientMessagePrivate::ClientMessagePrivate(const QString &method_name,
                                           const QVariantList &parameters)
  : id(-1), method_name(method_name), parameters(parameters) {

}

ClientMessage::ClientMessage(const QString &method_name,
                             const QVariantList &parameters)
  : d(new ClientMessagePrivate(method_name, parameters)) {

}

ClientMessage::~ClientMessage() {
  if(d) {
    delete d;
    d = 0;
  }
}

QString ClientMessage::name() const {
  return d->method_name;
}

QVariantList ClientMessage::parameters() const {
  return d->parameters;
}

} //namespace binaryrpc
