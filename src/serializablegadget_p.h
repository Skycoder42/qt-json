#pragma once

#include "serializablegadget.h"

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

    inline static QStringList keys(const Map &map) {
        QStringList keys;
        keys.reserve(static_cast<int>(map.size()));
        for (const auto &entry : map)
            keys.append(entry.first.toString());
        return keys;
    }
};

}
