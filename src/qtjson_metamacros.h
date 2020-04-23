#pragma once

#include "iserializable.h"

#include <type_traits>

#include <QtCore/qobjectdefs.h>

#define QTJSON_SERIALIZABLE_KEY "__qtjson_serializable"
#define QTJSON_SERIALIZABLE Q_CLASSINFO(QTJSON_SERIALIZABLE_KEY, "true")

#define QTJSON_PROPERTY_OFFSET_KEY "__qtjson_offset"
#define QTJSON_PROPERTY_OFFSET(offset) Q_CLASSINFO(QTJSON_PROPERTY_OFFSET_KEY, #offset)

#define QTJSON_SERIALIZABLE_PROP_KEY_STR "__qtjson_propcast_"
#define QTJSON_SERIALIZABLE_PROP_KEY(name) (QTJSON_SERIALIZABLE_PROP_KEY_STR #name)
#define QTJSON_SERIALIZABLE_PROP(name, ...) \
    Q_INVOKABLE static inline constexpr QtJson::ISerializable *__qtjson_propcast_ ## name(void *data) { \
        if constexpr (std::is_base_of_v<QtJson::ISerializable, __VA_ARGS__>) \
            return static_cast<QtJson::ISerializable*>(reinterpret_cast<__VA_ARGS__*>(data)); \
        else \
            return nullptr; \
	}

#define QTJSON_PROP(name, ...) \
	public: \
		__VA_ARGS__ name; \
	private: \
		Q_PROPERTY(__VA_ARGS__ name MEMBER name STORED true) \
		QTJSON_SERIALIZABLE_PROP(name, __VA_ARGS__)
