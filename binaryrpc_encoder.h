#ifndef BINARYRPC_ENCODER_H
#define BINARYRPC_ENCODER_H

#include "binaryrpc/binaryrpc_request.h"
#include "binaryrpc/binaryrpc_response.h"

namespace binaryrpc {

class BINARYRPC_EXPORT BinaryEncoder {
  QByteArray data;
  int pos;

public:

  BinaryEncoder();

  void resize(int size);

  operator QByteArray() const;

  QByteArray getBuff() const;

  BinaryEncoder &operator <<(char p);

  BinaryEncoder &operator <<(qint32 p);

  BinaryEncoder &operator <<(quint32 p);

  BinaryEncoder &operator <<(qint64 p);

  BinaryEncoder &operator <<(quint64 p);

  BinaryEncoder &operator <<(double p);

  BinaryEncoder &operator <<(const QString &p);

  BinaryEncoder &operator <<(const QByteArray &p);

  BinaryEncoder &operator <<(const QVariant &p);

  BinaryEncoder &operator <<(const Header &p);

  BinaryEncoder &operator <<(const Request &p);

  BinaryEncoder &operator <<(const Response &p);
};

} // namespace binaryrpc

#endif // BINARYRPC_ENCODER_H
