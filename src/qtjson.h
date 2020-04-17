#ifndef QTJSON_H
#define QTJSON_H

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

template <typename TValue>
inline QJsonValue stringify(const TValue &value, const Configuration &configuration = {}) {
    return stringify(QVariant::fromValue(value), configuration);
}

template <typename TValue>
inline QCborValue binarify(const TValue &value, const Configuration &configuration = {}) {
    return binarify(QVariant::fromValue(value), configuration);}
};

#endif // QTJSON_H
