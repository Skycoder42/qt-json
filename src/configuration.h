#pragma once

namespace QtJson {

struct CommonConfiguration {
    bool keepObjectName = true;
    bool enumAsString = true;
};

struct JsonConfiguration : public CommonConfiguration {

};

struct CborConfiguration : public CommonConfiguration {

};

}

