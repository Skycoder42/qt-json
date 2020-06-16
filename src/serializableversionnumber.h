#pragma once

#include "qtjson_global.h"
#include "iserializable.h"
#include "serializableadapter.h"

#include <QtCore/QVersionNumber>

namespace QtJson {

template <>
class QTJSON_EXPORT SerializableAdapter<QVersionNumber, void>
{
public:
	static QJsonValue toJson(const QVersionNumber &value, const Configuration &config = {});
	static QVersionNumber fromJson(const QJsonValue &value, const Configuration &config = {});
	static QCborValue toCbor(const QVersionNumber &value, const Configuration &config = {});
	static QVersionNumber fromCbor(const QCborValue &value, const Configuration &config = {});
};

}
