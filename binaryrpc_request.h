#ifndef BINARYRPC_REQUEST_H
#define BINARYRPC_REQUEST_H

#include <QtCore/QVariant>
#include "binaryrpc/binaryrpc_header.h"

namespace binaryrpc {

class BINARYRPC_EXPORT Request : public Header {
public:
  Request();

  Request(int id, const QString &method_name, const QList<QVariant> &params);

  bool isNull() const;

  void setMethodName(const QString &method_name);

  void setParameters(const QVariantList &parameters);

  QString getMethodName() const;

  QVariantList getParameters() const;

  void calcLength();

private:

  QString method_name;
  QVariantList parameters;
};

} //namespace binaryrpc

#endif //BINARYRPC_REQUEST_H
