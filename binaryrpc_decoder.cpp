#include <QtCore/QtEndian>
#include "binaryrpc/binaryrpc_decoder.h"

namespace binaryrpc {

BinaryDecoder::BinaryDecoder(const QByteArray &data) : data(data), pos(0) {

}

void BinaryDecoder::addBody(const QByteArray &data) {
  this->data += data;
}

bool BinaryDecoder::decode(char &p) {
  if(pos + 1 > data.size())
    return false;

  p = data[pos++];

  return true;
}

bool BinaryDecoder::decode(qint32 &p) {
  if(pos + 4 > data.size())
    return false;

  qint32 v(0);
  char *c_pt(reinterpret_cast<char *>(&v));
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *c_pt = data[pos++];
  p = qFromBigEndian(v);

  return true;
}

bool BinaryDecoder::decode(quint32 &p) {
  if(pos + 4 > data.size())
    return false;

  quint32 v(0);
  char *c_pt(reinterpret_cast<char *>(&v));
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *c_pt = data[pos++];
  p = qFromBigEndian(v);

  return true;
}

bool BinaryDecoder::decode(qint64 &p) {
  if(pos + 8 > data.size())
    return false;

  qint64 v(0);
  char *c_pt(reinterpret_cast<char *>(&v));
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *c_pt = data[pos++];
  p = qFromBigEndian(v);

  return true;
}

bool BinaryDecoder::decode(quint64 &p) {
  if(pos + 8 > data.size())
    return false;

  quint64 v(0);
  char *c_pt(reinterpret_cast<char *>(&v));
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *c_pt = data[pos++];
  p = qFromBigEndian(v);

  return true;
}

bool BinaryDecoder::decode(double &p) {
  if(pos + 8 > data.size()) return false;

  char *c_pt(reinterpret_cast<char *>(&p));
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *(c_pt++) = data[pos++];
  *c_pt = data[pos++];

  return true;
}

bool BinaryDecoder::decode(QString &p) {
  int temp_pos(pos);
  int data_size(data.size());
  while(1) {
    if(temp_pos >= data_size)
      return false;
    if(data[temp_pos++] == '\0')
      break;
  }
  p.clear();
  temp_pos -= pos + 1;
  p.resize(temp_pos);
  for(int i = 0; i < temp_pos; i++)
    p[i] = data[pos++];
  pos++;
  return true;
}

bool BinaryDecoder::decode(QByteArray &p, int length) {
  if((pos + length) > data.size())
    return false;

  p.resize(length);

  if(length > 0) {
    p = data.mid(pos, length);
    pos += length;
  }

  return true;
}

bool BinaryDecoder::decode(QDateTime &p) {
  QString string_date_time;

  if(decode(string_date_time) == false)
    return false;

  p = QDateTime::fromString(string_date_time, Qt::ISODate);

  return true;
}

bool BinaryDecoder::decode(QVariantMap &p) {
  int size(0);
  if(decode(size) == false)
    return false;
  QVariantMap temp;
  for(int i = 0; i < size; i++) {
    QString key;
    if(decode(key) == false)
      return false;
    QVariant value;
    if(decode(value) == false)
      return false;
    temp.insert(key, value);
  }
  p = temp;
  return true;
}

bool BinaryDecoder::decode(QVariantList &p) {
  int size(0);
  if(decode(size) == false)
    return false;
  QVariantList temp;
  for(int i = 0; i < size; i++) {
    QVariant value;
    if(decode(value) == false)
      return false;
    temp.push_back(value);
  }
  p = temp;
  return true;
}

bool BinaryDecoder::decode(QVariant &p) {
  char type(0);

  if(decode(type) == false)
    return false;

  switch(type) {
    case 0x00: {
      p = QVariant();
      return true;
    }
    case 0x01: {
      char temp(0);
      if(decode(temp) == false)
        return false;
      if(temp != 0x00 && temp != 0x01)
        return false;
      p = (bool)temp;
      return true;
    }
    case 0x02: {
      char temp(0);
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x03: {
      qint32 temp(0);
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x04: {
      quint32 temp(0);
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x05: {
      qint64 temp(0);
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x06: {
      quint64 temp(0);
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x07: {
      double temp(0);
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x08: {
      QString temp;
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x09: {
      int size_massive(0);
      if(decode(size_massive) == false)
        return false;
      QByteArray temp;
      if(decode(temp, size_massive) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x0A: {
      QDateTime temp;
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x0B: {
      QVariantMap temp;
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    case 0x0C: {
      QVariantList temp;
      if(decode(temp) == false)
        return false;
      p = temp;
      return true;
    }
    default: {
      return false;
    }
  }
}

bool BinaryDecoder::decode(Header &p) {
  int id(0);
  if(decode(id) == false)
    return false;

  int length(0);
  if(decode(length) == false)
    return false;

  p.setId(id);
  p.setLength(length);
  return true;
}

bool BinaryDecoder::decode(Request &p, bool decode_header) {
  if(decode_header) {
    if(decode((Header &)p) == false)
      return false;
  }

  QString method_name;
  if(decode(method_name) == false)
    return false;

  QVariantList parameters;
  while(pos < data.size()) {
    QVariant parameter;
    if(decode(parameter)) {
      parameters.push_back(parameter);
    } else {
      return false;
    }
  }

  p.setMethodName(method_name);
  p.setParameters(parameters);

  return true;
}

bool BinaryDecoder::decode(Response &p, bool decode_header) {
  if(decode_header) {
    if(decode((Header &)p) == false)
      return false;
  }

  char type(0);
  if(decode(type) == false)
    return false;

  if(type == 0x00) {
    QVariant return_value;
    if(decode(return_value) == false)
      return false;
    p.setFaultCode(-1);
    p.setFaultString(QString());
    p.setReturnValue(return_value);
    return true;
  } else if(type == 0x01) {
    int fault_code(0);
    if(decode(fault_code) == false)
      return false;
    QString fault_string;
    if(decode(fault_string) == false)
      return false;

    p.setFaultCode(fault_code);
    p.setFaultString(fault_string);
    p.setReturnValue(QVariant());
    return true;
  } else {
    return false;
  }
}

} // namespace binaryrpc
