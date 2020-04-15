#ifndef QTJSON_H
#define QTJSON_H

#include "qtjson_global.h"

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>
#include <QtCore/QVariant>

namespace QtJson
{

struct Configuration {
    bool keepObjectName = true;
    bool enumAsString = true;
};

QTJSON_EXPORT QJsonValue stringify(const QVariant &value, const Configuration &configuration = {});
QTJSON_EXPORT QCborValue binarify(const QVariant &value, const Configuration &configuration = {});
};

#endif // QTJSON_H
