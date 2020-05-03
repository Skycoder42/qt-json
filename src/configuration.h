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

}

Q_DECLARE_METATYPE(QtJson::CommonConfiguration)
Q_DECLARE_TYPEINFO(QtJson::CommonConfiguration, Q_MOVABLE_TYPE);

