#pragma once

#include <QtCore/QtGlobal>

#ifdef QTJSON_LIBRARY
# define QTJSON_EXPORT Q_DECL_EXPORT
#else
# define QTJSON_EXPORT Q_DECL_IMPORT
#endif
