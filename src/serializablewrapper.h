#pragma once

#include "serializablebytearray.h"
#include "serializabledatetime.h"
#include "serializablearray.h"
#include "serializabledictionary.h"
#include "serializableoptional.h"
#include "serializableversionnumber.h"

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

template <typename T>
struct SerializableWrapper<QList<T>> {
	using type = SerializableList<T>;
};

template <typename T>
struct SerializableWrapper<QVector<T>> {
	using type = SerializableVector<T>;
};

#ifndef QT_NO_LINKED_LIST
template <typename T>
struct SerializableWrapper<QLinkedList<T>> {
	using type = SerializableLinkedList<T>;
};
#endif

template <typename T>
struct SerializableWrapper<QQueue<T>> {
	using type = SerializableQueue<T>;
};

template <typename T>
struct SerializableWrapper<QStack<T>> {
	using type = SerializableStack<T>;
};

template <typename T>
struct SerializableWrapper<QSet<T>> {
	using type = SerializableSet<T>;
};

template <typename TKey, typename TValue>
struct SerializableWrapper<QHash<TKey, TValue>> {
	using type = SerializableHash<TKey, TValue>;
};

template <typename TKey, typename TValue>
struct SerializableWrapper<QMap<TKey, TValue>> {
	using type = SerializableMap<TKey, TValue>;
};

template <typename T>
struct SerializableWrapper<std::optional<T>> {
    using type = SerializableOptional<T>;
};

template <>
struct SerializableWrapper<QVersionNumber> {
    using type = SerializableVersionNumber;
};

}
