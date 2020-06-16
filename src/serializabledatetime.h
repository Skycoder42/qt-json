#pragma once

#include "qtjson_global.h"
#include "iserializable.h"
#include "serializableadapter.h"

#include <QtCore/QDateTime>

namespace QtJson {

template <>
class QTJSON_EXPORT SerializableAdapter<QDateTime, void>
{
public:
	static QJsonValue toJson(const QDateTime &value, const Configuration &config = {});
	static QDateTime fromJson(const QJsonValue &value, const Configuration &config = {});
	static QCborValue toCbor(const QDateTime &value, const Configuration &config = {});
	static QDateTime fromCbor(const QCborValue &value, const Configuration &config = {});
};

}

