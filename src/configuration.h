#pragma once

#include <QtCore/QVariantMap>

namespace QtJson {

struct CommonConfiguration {
	bool keepObjectName = true;
	bool enumAsString = true;

	QVariantMap extraConfig;
};

struct JsonConfiguration : public CommonConfiguration {

};

struct CborConfiguration : public CommonConfiguration {

};

}

