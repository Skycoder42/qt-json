#pragma once

#include "serializablegadget.h"

#include <QtCore/QMetaMethod>

namespace QtJson::__private {

template <>
struct DataValueInfo<QJsonValue> {
	using Value = QJsonValue;
	using Map = QJsonObject;
	using List = QJsonArray;

	static constexpr auto Undefined = QJsonValue::Undefined;
	static constexpr auto Null = QJsonValue::Null;
	static constexpr auto String = QJsonValue::String;
	static constexpr auto Integer = QJsonValue::Double;

	static constexpr auto To = AdapterMethod::ToJson;
	static constexpr auto From = AdapterMethod::FromJson;

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

	static constexpr auto Undefined = QCborValue::Undefined;
	static constexpr auto Null = QCborValue::Null;
	static constexpr auto String = QCborValue::String;
	static constexpr auto Integer = QCborValue::Integer;

	static constexpr auto To = AdapterMethod::ToCbor;
	static constexpr auto From = AdapterMethod::FromCbor;

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

#define QTJSON_SERIALIZABLE_PROP_TO_JSON_SIGNATURE(name) "__qtjson_" + name + "_toJson(const QtJson::Configuration &) const"
#define QTJSON_SERIALIZABLE_PROP_FROM_JSON_SIGNATURE(name) "__qtjson_" + name + "_fromJson(const QJsonValue &, const QtJson::Configuration &)"
#define QTJSON_SERIALIZABLE_PROP_TO_CBOR_SIGNATURE(name) "__qtjson_" + name + "_toCbor(const QtJson::Configuration &) const"
#define QTJSON_SERIALIZABLE_PROP_FROM_CBOR_SIGNATURE(name) "__qtjson_" + name + "_fromCbor(const QCborValue &, const QtJson::Configuration &)"
