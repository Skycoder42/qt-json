#pragma once

#include "qtjson_global.h"

#include <type_traits>

#include <QtCore/QtContainerFwd>

namespace QtJson::__private {

template <template <typename...> class TDictionary>
struct is_dictionary : public std::false_type {};

template <template <typename...> class TDictionary>
constexpr auto is_dictionary_v = is_dictionary<TDictionary>::value;



template <>
struct is_dictionary<QMap> : public std::true_type {};
template <>
struct is_dictionary<QHash> : public std::true_type {};

}
