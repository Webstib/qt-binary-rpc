#ifndef BINARYRPC_GLOBAL_H
#define BINARYRPC_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/QCoreApplication>

#define BRPC_ERR_CODE_UNKNOWN      0x00000000
#define BRPC_ERR_CODE_SOCK         0x00000001
#define BRPC_ERR_CODE_DECODE       0x00000002
#define BRPC_ERR_CODE_NOT_EXEC     0x00000003
#define BRPC_ERR_CODE_NOT_FOUND    0x00000004
#define BRPC_ERR_CODE_EXEC_TIMEOUT 0x00000005
#define BRPC_ERR_CODE_SQL          0x00000006

static const char BRPC_ERR_CODE_NOT_FOUND_D[] = "Method not found";

#if defined(BINARYRPC_LIBRARY)
#  define BINARYRPC_EXPORT Q_DECL_EXPORT
#else
#  define BINARYRPC_EXPORT Q_DECL_IMPORT
#endif

#endif // BINARYRPC_GLOBAL_H
