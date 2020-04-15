#ifndef QTJSON_H
#define QTJSON_H

#include "qtjson_global.h"

#include <QtCore/QJsonObject>
#include <QtCore/QVariant>

namespace QtJson
{

struct QTJSON_EXPORT Configuration {
    bool keepObjectName = true;
    bool enumAsString = true;
};

QTJSON_EXPORT QJsonValue stringify(const QVariant &value, Configuration configuration = {});
};

#endif // QTJSON_H
