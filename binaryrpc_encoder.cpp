#include <QtCore/QtEndian>
#include <QtCore/QDateTime>
#include "binaryrpc/binaryrpc_encoder.h"

namespace binaryrpc {

BinaryEncoder::BinaryEncoder() : pos(-1) {

}

void BinaryEncoder::resize(int size) {
  data.clear();
  if(size > 0) {
    data.resize(size);
  }
  pos = 0;
}

BinaryEncoder::operator QByteArray() const {
  return data;
}

QByteArray BinaryEncoder::getBuff() const {
  return data;
}

BinaryEncoder &BinaryEncoder::operator <<(char p) {
  data[pos++] = p;
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(qint32 p) {
  qint32 v(qToBigEndian(p));
  char *c_pt(reinterpret_cast<char *>(&v));
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *c_pt;
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(quint32 p) {
  quint32 v(qToBigEndian(p));
  char *c_pt(reinterpret_cast<char *>(&v));
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *c_pt;
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(qint64 p) {
  qint64 v(qToBigEndian(p));
  char *c_pt(reinterpret_cast<char *>(&v));
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *c_pt;
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(quint64 p) {
  quint64 v(qToBigEndian(p));
  char *c_pt(reinterpret_cast<char *>(&v));
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *c_pt;
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(double p) {
  char *c_pt(reinterpret_cast<char *>(&p));
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *(c_pt++);
  data[pos++] = *c_pt;
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(const QString &p) {
  *this << (p.toUtf8() + '\0');
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(const QByteArray &p) {
  for(int i = 0; i < p.size(); i++)
    data[pos++] = p[i];
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(const QVariant &p) {
  switch(p.type()) {
    case QVariant::Invalid:
      *this << char(0x00);
      break;
    case QVariant::Bool:
      *this << char(0x01);
      *this << char(p.toBool());
      break;
    case QVariant::Char:
      *this << char(0x02);
      *this << p.toChar().toLatin1();
      break;
    case QVariant::Int:
      *this << char(0x03);
      *this << p.toInt();
      break;
    case QVariant::UInt:
      *this << char(0x04);
      *this << p.toUInt();
      break;
    case QVariant::LongLong:
      *this << char(0x05);
      *this << p.toLongLong();
      break;
    case QVariant::ULongLong:
      *this << char(0x06);
      *this << p.toULongLong();
      break;
    case QVariant::Double:
      *this << char(0x07);
      *this << p.toDouble();
      break;
    case QVariant::String:
      *this << char(0x08);
      *this << p.toString();
      break;
    case QVariant::ByteArray:
      *this << char(0x09);
      *this << p.toByteArray().size();
      *this << p.toByteArray();
      break;
    case QVariant::DateTime:
      *this << char(0x0A);
      *this << p.toDateTime().toString("yyyyMMddThhmmsszzz");
      break;
    case QVariant::Map: {
      QVariantMap p_map(p.toMap());
      *this << char(0x0B);
      *this << p_map.size();
      for(QVariantMap::iterator it = p_map.begin(); it != p_map.end(); it++) {
        *this << it.key();
        *this << it.value();
      }
      break;
    }
    case QVariant::List: {
      QVariantList p_list(p.toList());
      *this << char(0x0C);
      *this << p_list.size();
      foreach(QVariant v, p_list) {
        *this << v;
      }
      break;
    }
    default:
      break;
  }
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(const Header &p) {
  *this << p.getId();
  *this << p.getLength();
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(const Request &p) {
  resize(p.getLength());
  *this << (Header)p;
  *this << p.getMethodName();
  foreach(QVariant parameter, p.getParameters()) {
    *this << parameter;
  }
  return *this;
}

BinaryEncoder &BinaryEncoder::operator <<(const Response &p) {
  resize(p.getLength());
  *this << (Header)p;
  if(p.isFault()) {
    *this << char(0x01);
    *this << p.getFaultCode();
    *this << p.getFaultString();
  } else {
    *this << char(0x00);
    *this << p.getReturnValue();
  }
  return *this;
}

} // namespace binaryrpc
