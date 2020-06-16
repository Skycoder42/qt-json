#pragma once

#include "qtjson_global.h"

#include <QtCore/QVariantMap>

namespace QtJson {

enum class ByteArrayMode {
	Base64,
	Base64url,
	Hex
};

enum class ValidationFlag {
	None = 0x00, // Do not perform extra validation, only make sure types are valid and compatible
	NoExtra = 0x01, // Make sure the json/cbor does not contain any properties that are not in the type to deserialize it to
	NoMissing = 0x02, // Make sure all properties of the type have a value in the json/cbor data

	Full = (NoExtra | NoMissing), // Validate properties are exactly the same as declared
};
Q_DECLARE_FLAGS(ValidationFlags, ValidationFlag)

struct Configuration {
	bool ignoreStored = false;
	ByteArrayMode byteArrayMode = ByteArrayMode::Base64;
	bool dateAsTimeStamp = false;
	bool enumAsString = true;
    bool versionAsString = true;
	ValidationFlags validation = ValidationFlag::Full;

	QVariantMap extraConfig;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(QtJson::ValidationFlags)

Q_DECLARE_METATYPE(QtJson::Configuration)
Q_DECLARE_TYPEINFO(QtJson::Configuration, Q_MOVABLE_TYPE);

