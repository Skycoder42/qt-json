#pragma once

#include "serializablebytearray.h"
#include "serializabledatetime.h"
#include "serializablearray.h"
#include "serializabledictionary.h"
#include "serializableoptional.h"
#include "serializableversionnumber.h"

namespace QtJson {

template <typename TType, typename = void>
struct SerializableCast;

template <typename TType>
struct SerializableCast<TType, std::enable_if_t<!std::is_base_of_v<ISerializable, TType>, void>> {
	static inline constexpr ISerializable *cast(TType *) {
		return nullptr;
	}
};

template <typename TType>
struct SerializableCast<TType, std::enable_if_t<std::is_base_of_v<ISerializable, TType>, void>> {
	static inline constexpr ISerializable *cast(TType *ptr) {
		return static_cast<ISerializable*>(ptr);
	}
};

}
