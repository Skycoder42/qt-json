#ifndef QT_JSON_P_H
#define QT_JSON_P_H

#include "qtjson.h"

namespace QtJson::__private {

QTJSON_EXPORT ISerializableWrapperFactory *getWrapperFactory(int typeId);

}

#endif // QT_JSON_P_H
