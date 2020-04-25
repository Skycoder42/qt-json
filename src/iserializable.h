#pragma once

#include "configuration.h"

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>

namespace QtJson {

class QTJSON_EXPORT ISerializable
{
public:
	virtual ~ISerializable();

	virtual QJsonValue toJson(const JsonConfiguration &config = {}) const = 0;
	virtual void assignJson(const QJsonValue &value, const JsonConfiguration &config = {}) = 0;

	virtual QCborValue toCbor(const CborConfiguration &config = {}) const = 0;
	virtual void assignCbor(const QCborValue &value, const CborConfiguration &config = {}) = 0;
};

}

Q_DECLARE_METATYPE(QtJson::ISerializable*)
