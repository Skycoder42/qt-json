#ifndef QTJSON_GLOBAL_H
#define QTJSON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTJSON_LIBRARY)
#  define QTJSON_EXPORT Q_DECL_EXPORT
#else
#  define QTJSON_EXPORT Q_DECL_IMPORT
#endif

#endif // QTJSON_GLOBAL_H
