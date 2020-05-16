#pragma once

#include "iserializable.h"
#include "serializableadapter.h"
#include "qtjson_exception.h"
#include "qtjson_helpers.h"
#include "serializabledictionary_p.h"

#include <QtCore/QtContainerFwd>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>

namespace QtJson {

// TODO move to enum
constexpr QCborTag MapTypeTag = static_cast<QCborTag>(259);

template <typename TKey, typename TValue, template <typename...> class TDictionary, typename... TExtra>
class SerializableAdapter<TDictionary<TKey, TValue, TExtra...>, std::enable_if_t<__private::is_dictionary_v<TDictionary>, void>>
{
public:
	using dictionary_type = TDictionary<TKey, TValue, TExtra...>;

	static QJsonValue toJson(const dictionary_type &value, const Configuration &config = {}) {
		QJsonObject object;
		for (auto it = value.begin(), end = value.end(); it != end; ++it) {
			object.insert(QVariant::fromValue(it.key()).toString(),
						  SerializableAdapter<TValue>::toJson(it.value(), config));
		}
		return object;
	}

	static dictionary_type fromJson(const QJsonValue &value, const Configuration &config = {}) {
		if (!value.isObject())
			throw InvalidValueTypeException{value.type(), {QJsonValue::Object}};
		const auto object = value.toObject();
		dictionary_type result;
		for (auto it = object.begin(), end = object.end(); it != end; ++it) {
			result.insert(QVariant{it.key()}.template value<TKey>(),
						  SerializableAdapter<TValue>::fromJson(it.value(), config));
		}
        return result;
	}

	static QCborValue toCbor(const dictionary_type &value, const Configuration &config = {}) {
		QCborMap map;
		for (auto it = value.begin(), end = value.end(); it != end; ++it) {
			map.insert(SerializableAdapter<TKey>::toCbor(it.key(), config),
					   SerializableAdapter<TValue>::toCbor(it.value(), config));
		}
		return {MapTypeTag, map};
	}

	static dictionary_type fromCbor(const QCborValue &value, const Configuration &config = {}) {
		const auto xValue = __private::extract(value);
		if (!xValue.isMap())
			throw InvalidValueTypeException{value.type(), {QCborValue::Map}};
		const auto map = xValue.toMap();
		dictionary_type result;
		for (auto it = map.begin(), end = map.end(); it != end; ++it) {
			result.insert(SerializableAdapter<TKey>::fromCbor(it.key(), config),
						  SerializableAdapter<TValue>::fromCbor(it.value(), config));
		}
        return result;
	}
};

}
