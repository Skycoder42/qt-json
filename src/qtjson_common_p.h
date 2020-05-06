#pragma once

#include "configuration.h"

#include <optional>

#include <QtCore/QMetaClassInfo>
#include <QtCore/QVariant>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QCborValue>
#include <QtCore/QCborMap>

namespace QtJson::__private {

template <typename TType>
struct DataValueInfo;

template <>
struct DataValueInfo<QJsonValue> {
	using Value = QJsonValue;
	using Map = QJsonObject;
	using List = QJsonArray;

	static constexpr auto Undefined = QJsonValue::Undefined;
	static constexpr auto Null = QJsonValue::Null;

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

	inline static QStringList keys(const Map &map) {
		QStringList keys;
		keys.reserve(static_cast<int>(map.size()));
		for (const auto &entry : map)
			keys.append(entry.first.toString());
		return keys;
	}
};

QTJSON_EXPORT QVariant findInfo(const QMetaObject *metaObject, const char *key, const QVariant &defaultValue = {});

template <typename T>
std::optional<T> findInfo(const QMetaObject *metaObject, const char *key) {
	const auto info = findInfo(metaObject, key);
	if (info.isValid())
		return info.template value<T>();
	else
		return std::nullopt;
}

template <typename T>
T findInfo(const QMetaObject *metaObject, const char *key, const T &defaultValue) {
	return findInfo(metaObject, key, QVariant::fromValue(defaultValue)).template value<T>();
}

QTJSON_EXPORT QCborValue extract(const QCborValue &value, QCborTag *innerTag = nullptr, bool skipInner = false);

QTJSON_EXPORT void verifyTagImpl(QCborTag tag, bool allowUntagged = true, const QList<QCborTag> &expected = {});

template <typename... TArgs>
inline void verifyTag(QCborTag tag, bool allowUntagged, TArgs&&... args) {
    verifyTagImpl(tag, allowUntagged, QList<QCborTag>{std::forward<TArgs>(args)...});
}

}
