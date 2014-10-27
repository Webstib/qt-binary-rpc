#ifndef BINARYRPC_HEADER_H
#define BINARYRPC_HEADER_H

#include <QtCore/QByteArray>
#include "binaryrpc/binaryrpc_global.h"

namespace binaryrpc {

class BINARYRPC_EXPORT Header {
public:
  Header(int id);

  int getId() const;

  int getLength() const;

  void setId(int id);

  void setLength(int length);

  static const int HEADER_LENGTH = 8;

protected:

  int id;
  int length;
};

} //namespace binaryrpc

#endif //BINARYRPC_HEADER_H
