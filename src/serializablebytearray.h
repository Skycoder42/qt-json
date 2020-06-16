#pragma once

#include "qtjson_global.h"
#include "iserializable.h"
#include "serializableadapter.h"

#include <QtCore/QByteArray>

namespace QtJson {

template <>
class QTJSON_EXPORT SerializableAdapter<QByteArray, void>
{
public:
	static QJsonValue toJson(const QByteArray &value, const Configuration &config = {});
	static QByteArray fromJson(const QJsonValue &value, const Configuration &config = {});
	static QCborValue toCbor(const QByteArray &value, const Configuration &config = {});
	static QByteArray fromCbor(const QCborValue &value, const Configuration &config = {});
};

}
