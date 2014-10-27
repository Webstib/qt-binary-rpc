TARGET       = binary_rpc

include(../../library.pri)

QT          += network

DEFINES     += BINARYRPC_LIBRARY

INCLUDEPATH += $$PWD

HEADERS += \
  binaryrpc_global.h \
  binaryrpc_client.h \
  binaryrpc_client_p.h \
  binaryrpc_header.h \
  binaryrpc_request.h \
  binaryrpc_response.h \
  binaryrpc_server.h \
  binaryrpc_server_p.h \
  binaryrpc_session.h \
  binaryrpc_client_message.h \
  binaryrpc_client_message_p.h \
  binaryrpc_server_message.h \
  binaryrpc_server_message_p.h \
  binaryrpc_session_p.h \
  binaryrpc_encoder.h \
  binaryrpc_decoder.h \
    binaryrpc_helper.h

SOURCES += \
  binaryrpc_client.cpp \
  binaryrpc_header.cpp \
  binaryrpc_request.cpp \
  binaryrpc_response.cpp \
  binaryrpc_server.cpp \
  binaryrpc_session.cpp \
  binaryrpc_client_message.cpp \
  binaryrpc_server_message.cpp \
  binaryrpc_encoder.cpp \
  binaryrpc_decoder.cpp















































































































