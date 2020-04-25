#pragma once

#include "serializablebytearray.h"
#include "serializabledatetime.h"

namespace QtJson {

template <typename TType>
struct SerializableWrapper {
	using type = TType;
};

template <>
struct SerializableWrapper<QByteArray> {
	using type = SerializableByteArray;
};

template <>
struct SerializableWrapper<QDateTime> {
	using type = SerializableDateTime;
};

}
