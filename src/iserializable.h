#pragma once

#include "qtjson_global.h"
#include "configuration.h"

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>

namespace QtJson {

class QTJSON_EXPORT ISerializable
{
public:
	virtual ~ISerializable();

    virtual QJsonValue toJson(const Configuration &config = {}) const = 0;
    virtual void assignJson(const QJsonValue &value, const Configuration &config = {}) = 0;

    virtual QCborValue toCbor(const Configuration &config = {}) const = 0;
    virtual void assignCbor(const QCborValue &value, const Configuration &config = {}) = 0;
};

}

Q_DECLARE_METATYPE(QtJson::ISerializable*)
