#pragma once

#include "qtjson_global.h"

#include <type_traits>

#include <QtCore/QtContainerFwd>

namespace QtJson::__private {

template <typename, typename = void>
struct has_reserve : public std::false_type {};

template <typename T>
struct has_reserve<T, std::void_t<decltype(std::declval<T>().reserve(0))>> : std::true_type {};

template <typename T>
inline constexpr bool has_reserve_v = has_reserve<T>::value;

template <template <typename...> class TArray>
struct is_array : public std::false_type {};

template <template <typename...> class TArray>
constexpr auto is_array_v = is_array<TArray>::value;



template <>
struct is_array<QList> : public std::true_type {};
template <>
struct is_array<QVector> : public std::true_type {};
#ifndef QT_NO_LINKED_LIST
template <>
struct is_array<QLinkedList> : public std::true_type {};
#endif
template <>
struct is_array<QStack> : public std::true_type {};
template <>
struct is_array<QQueue> : public std::true_type {};

}
