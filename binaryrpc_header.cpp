#include "binaryrpc/binaryrpc_header.h"

namespace binaryrpc {

Header::Header(int id) : id(id), length(-1) {

}

int Header::getId() const {
  return id;
}

int Header::getLength() const {
  return length;
}

void Header::setId(int id) {
  this->id = id;
}

void Header::setLength(int length) {
  this->length = length;
}

} //namespace binaryrpc
