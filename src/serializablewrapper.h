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

template <typename T>
struct SerializableWrapper<QList<T>> {
	using type = SerializableList<typename SerializableWrapper<T>::type>;
};

template <typename T>
struct SerializableWrapper<QVector<T>> {
	using type = SerializableVector<typename SerializableWrapper<T>::type>;
};

#ifndef QT_NO_LINKED_LIST
template <typename T>
struct SerializableWrapper<QLinkedList<T>> {
	using type = SerializableLinkedList<typename SerializableWrapper<T>::type>;
};
#endif

template <typename T>
struct SerializableWrapper<QQueue<T>> {
	using type = SerializableQueue<typename SerializableWrapper<T>::type>;
};

template <typename T>
struct SerializableWrapper<QStack<T>> {
	using type = SerializableStack<typename SerializableWrapper<T>::type>;
};

template <typename T>
struct SerializableWrapper<QSet<T>> {
	using type = SerializableSet<typename SerializableWrapper<T>::type>;
};

template <typename TKey, typename TValue>
struct SerializableWrapper<QHash<TKey, TValue>> {
	using type = SerializableHash<typename SerializableWrapper<TKey>::type,
								  typename SerializableWrapper<TValue>::type>;
};

template <typename TKey, typename TValue>
struct SerializableWrapper<QMap<TKey, TValue>> {
	using type = SerializableMap<typename SerializableWrapper<TKey>::type,
								 typename SerializableWrapper<TValue>::type>;
};

template <typename T>
struct SerializableWrapper<std::optional<T>> {
	using type = SerializableOptional<typename SerializableWrapper<T>::type>;
};

template <>
struct SerializableWrapper<QVersionNumber> {
	using type = SerializableVersionNumber;
};

}
