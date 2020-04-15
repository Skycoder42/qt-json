#ifndef QTJSON_H
#define QTJSON_H

#include "qtjson_global.h"

#include <QtCore/QJsonValue>
#include <QtCore/QVariant>

namespace QtJson
{

struct Configuration {
    bool keepObjectName = true;
    bool enumAsString = true;
};

QTJSON_EXPORT QJsonValue stringify(const QVariant &value, Configuration configuration = {});
// TODO CBOR?
};

#endif // QTJSON_H
