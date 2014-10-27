#ifndef BINARYRPC_RESPONSE_H
#define BINARYRPC_RESPONSE_H

#include <QtCore/QVariant>
#include "binaryrpc/binaryrpc_header.h"

namespace binaryrpc {

class BINARYRPC_EXPORT Response : public Header {
public:
  Response();

  Response(int id, const QVariant &return_value);

  Response(int id, int fault_code, const QString &fault_string);

  bool isNull() const;

  bool isFault() const;

  void setFaultCode(int fault_code);

  void setFaultString(const QString &fault_string);

  void setReturnValue(const QVariant &return_value);

  int getFaultCode() const;

  QString getFaultString() const;

  QVariant getReturnValue() const;

  void calcLength();

private:

  int fault_code;
  QString fault_string;
  QVariant return_value;
};

} //namespace binaryrpc

#endif //BINARYRPC_RESPONSE_H
