#ifndef BINARYRPC_HELPER_H
#define BINARYRPC_HELPER_H

#include <QtCore/QDateTime>
#include <QtCore/QVariant>

namespace binaryrpc {

static inline int getSize(const QVariant &p) {
  switch(p.type()) {
    case QVariant::Invalid:
      return 1;
    case QVariant::Bool:
    case QVariant::Char:
      return 2;
    case QVariant::Int:
    case QVariant::UInt:
      return 5;
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::Double:
      return 9;
    case QVariant::String:
      return 2 + p.toString().size();
    case QVariant::ByteArray:
      return 5 + p.toByteArray().size();
    case QVariant::DateTime:
      return 2 + p.toDateTime().toString("yyyyMMddhhmmsszzz").size();
    case QVariant::Map: {
      QVariantMap p_map(p.toMap());
      int size = 5;
      for(QVariantMap::iterator it = p_map.begin(); it != p_map.end(); it++) {
        size += it.key().size() + 1 + getSize(it.value());
      }
      return size;
    }
    case QVariant::List: {
      QVariantList p_list(p.toList());
      int size = 5;
      foreach(QVariant v, p_list) {
        size += getSize(v);
      }
      return size;
    }
    default:
      return 0;
  }
}

} // namespace binaryrpc

#endif // BINARYRPC_HELPER_H
