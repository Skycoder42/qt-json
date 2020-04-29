#pragma once

#include <QtCore/QVariantMap>

namespace QtJson {

enum class ByteArrayMode {
	Base64,
	Base64url,
	Hex
};

struct CommonConfiguration {
	bool keepObjectName = false;
	bool enumAsString = true;
	ByteArrayMode byteArrayMode = ByteArrayMode::Base64;
	bool dateAsTimeStamp = false;
	bool ignoreStored = false;

	QVariantMap extraConfig;
};

using JsonConfiguration = CommonConfiguration;

using CborConfiguration = CommonConfiguration;

}

