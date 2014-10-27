#ifndef BINARYRPC_DECODER_H
#define BINARYRPC_DECODER_H

#include <QtCore/QDateTime>
#include "binaryrpc/binaryrpc_global.h"
#include "binaryrpc/binaryrpc_request.h"
#include "binaryrpc/binaryrpc_response.h"

namespace binaryrpc {

class BINARYRPC_EXPORT BinaryDecoder {
  QByteArray data;
  int pos;

public:

  BinaryDecoder(const QByteArray &data);

  void addBody(const QByteArray &data);

  bool decode(char &p);

  bool decode(qint32 &p);

  bool decode(quint32 &p);

  bool decode(qint64 &p);

  bool decode(quint64 &p);

  bool decode(double &p);

  bool decode(QString &p);

  bool decode(QByteArray &p, int length);

  bool decode(QDateTime &p);

  bool decode(QVariant &p);

  bool decode(QVariantMap &p);

  bool decode(QVariantList &p);

  bool decode(Header &p);

  bool decode(Request &p, bool decode_header = false);

  bool decode(Response &p, bool decode_header = false);
};

} // namespace binaryrpc

#endif // BINARYRPC_DECODER_H
