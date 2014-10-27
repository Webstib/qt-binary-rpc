#include "binaryrpc/binaryrpc_response.h"
#include "binaryrpc/binaryrpc_helper.h"

namespace binaryrpc {

Response::Response() : Header(-1), fault_code(-1) {

}

Response::Response(int id, const QVariant &return_value)
  : Header(id), fault_code(-1), return_value(return_value) {

}

Response::Response(int id, int fault_code, const QString &fault_string)
  : Header(id), fault_code(fault_code), fault_string(fault_string) {

}

bool Response::isNull() const {
  return (fault_code == -1 && fault_string.isEmpty() && return_value.isNull());
}

bool Response::isFault() const {
  return (fault_code != -1);
}

void Response::setFaultCode(int fault_code) {
  this->fault_code = fault_code;
}

void Response::setFaultString(const QString &fault_string) {
  this->fault_string = fault_string;
}

void Response::setReturnValue(const QVariant &return_value) {
  this->return_value = return_value;
}

int Response::getFaultCode() const {
  return fault_code;
}

QString Response::getFaultString() const {
  return fault_string;
}

QVariant Response::getReturnValue() const {
  return return_value;
}

void Response::calcLength() {
  if(isNull() == false) {
    length = Header::HEADER_LENGTH + 1;
    if(isFault()) {
      length += 5 + fault_string.size();
    } else {
      length += getSize(return_value);
    }
  }
}

} //namespace binaryrpc
