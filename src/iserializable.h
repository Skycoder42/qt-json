#ifndef QTJSON_ISERIALIZABLE_H
#define QTJSON_ISERIALIZABLE_H

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>

namespace QtJson {

class QTJSON_EXPORT ISerializable
{
public:
    virtual ~ISerializable();

    virtual QJsonValue toJson() const = 0;
    virtual void assignJson(const QJsonValue &value) = 0;

    virtual QCborValue toCbor() const = 0;
    virtual void assignCbor(const QCborValue &value) = 0;
};

}

#endif // QTJSON_ISERIALIZABLE_H
