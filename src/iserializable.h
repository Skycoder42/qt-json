#pragma once

#include "configuration.h"

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>

namespace QtJson {

class QTJSON_EXPORT ISerializable
{
public:
	virtual ~ISerializable();

	virtual QJsonValue toJson(const CommonConfiguration &config = {}) const = 0;
	virtual void assignJson(const QJsonValue &value, const CommonConfiguration &config = {}) = 0;

	virtual QCborValue toCbor(const CommonConfiguration &config = {}) const = 0;
	virtual void assignCbor(const QCborValue &value, const CommonConfiguration &config = {}) = 0;
};

}

Q_DECLARE_METATYPE(QtJson::ISerializable*)
