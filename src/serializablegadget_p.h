#pragma once

#include "qtjson_global.h"
#include "serializablegadget.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>

namespace QtJson::__private {

template <>
struct DataValueInfo<QJsonValue> {
	using Value = QJsonValue;
	using Map = QJsonObject;
	using List = QJsonArray;

    static constexpr auto Invalid = QJsonValue::Undefined;
	static constexpr auto Null = QJsonValue::Null;
	static constexpr auto String = QJsonValue::String;
    static constexpr auto Integer = QJsonValue::Double;

	inline static QReturnArgument<Value> returnArg(Value &value) {
		return QReturnArgument<Value>("QJsonValue", value);
	}

	inline static QStringList keys(const Map &map) {
		return map.keys();
	}
};

template <>
struct DataValueInfo<QCborValue> {
	using Value = QCborValue;
	using Map = QCborMap;
	using List = QCborArray;

    static constexpr auto Invalid = QCborValue::Invalid;
	static constexpr auto Null = QCborValue::Null;
	static constexpr auto String = QCborValue::String;
    static constexpr auto Integer = QCborValue::Integer;

	inline static QReturnArgument<Value> returnArg(Value &value) {
		return QReturnArgument<Value>("QCborValue", value);
	}

	inline static QStringList keys(const Map &map) {
		QStringList keys;
		keys.reserve(static_cast<int>(map.size()));
		for (const auto &entry : map)
			keys.append(entry.first.toString());
		return keys;
	}
};

}

#define QTJSON_SERIALIZABLE_PROP_TO_JSON_SIGNATURE(name) "__qtjson_" + name + "_toJson(const QtJson::Configuration &)"
#define QTJSON_SERIALIZABLE_PROP_FROM_JSON_SIGNATURE(name) "__qtjson_" + name + "_fromJson(const QJsonValue &, const QtJson::Configuration &)"
#define QTJSON_SERIALIZABLE_PROP_TO_CBOR_SIGNATURE(name) "__qtjson_" + name + "_toCbor(const QtJson::Configuration &)"
#define QTJSON_SERIALIZABLE_PROP_FROM_CBOR_SIGNATURE(name) "__qtjson_" + name + "_fromCbor(const QCborValue &, const QtJson::Configuration &)"
