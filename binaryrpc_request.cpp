#include "binaryrpc/binaryrpc_request.h"
#include "binaryrpc/binaryrpc_helper.h"

namespace binaryrpc {

Request::Request() : Header(-1) {

}

Request::Request(int id,
                 const QString &method_name,
                 const QVariantList &parameters)
    : Header(id),
      method_name(method_name),
      parameters(parameters) {

}

bool Request::isNull() const {
  return (method_name.isEmpty() && parameters.isEmpty());
}

void Request::setMethodName(const QString &method_name) {
  this->method_name = method_name;
}

void Request::setParameters(const QVariantList &parameters) {
  this->parameters = parameters;
}

QString Request::getMethodName() const {
  return method_name;
}

QList<QVariant> Request::getParameters() const {
  return parameters;
}

void Request::calcLength() {
  length = Header::HEADER_LENGTH + method_name.size() + 1;
  foreach(QVariant v, parameters) {
    length += getSize(v);
  }
}

} //namespace binaryrpc
