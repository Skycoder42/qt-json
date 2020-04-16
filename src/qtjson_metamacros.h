#ifndef QTJSON_METAMACROS_H
#define QTJSON_METAMACROS_H

#include <QtCore/qobjectdefs.h>

#define QTJSON_SERIALIZABLE_KEY "__qtjson_serializable"
#define QTJSON_SERIALIZABLE Q_CLASSINFO(QTJSON_SERIALIZABLE_KEY, "true")

#define QTJSON_PROPERTY_OFFSET_KEY "__qtjson_property_offset"
#define QTJSON_PROPERTY_OFFSET(offset) Q_CLASSINFO(QTJSON_PROPERTY_OFFSET_KEY, #offset)

#endif // QTJSON_METAMACROS_H
