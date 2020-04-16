#include "qtjson_common_p.h"

QVariant QtJson::findInfo(const QMetaObject *metaObject, const char *key, const QVariant &defaultValue)
{
    const auto cIdx = metaObject->indexOfClassInfo(key);
    if (cIdx < 0)
        return defaultValue;
    else
        return QVariant{QString::fromUtf8(metaObject->classInfo(cIdx).value())};
}
