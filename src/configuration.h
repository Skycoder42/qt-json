#pragma once

#include <QtCore/QVariantMap>

namespace QtJson {

enum class ByteArrayMode {
    Base64,
    Base64url,
    Hex
};

struct CommonConfiguration {
	bool keepObjectName = true;
	bool enumAsString = true;
    ByteArrayMode byteArrayMode = ByteArrayMode::Base64;

	QVariantMap extraConfig;
};

struct JsonConfiguration : public CommonConfiguration {

};

struct CborConfiguration : public CommonConfiguration {

};

}

