#include "qtjson_helpers.h"

QCborValue QtJson::__private::extract(const QCborValue &value, QCborTag *innerTag, bool skipInner)
{
    if (value.isTag()) {
        const auto inner = value.taggedValue();
        if (skipInner && !inner.isTag())
            return value;
        if (innerTag)
            *innerTag = value.tag();
        return extract(inner, innerTag, skipInner);
    } else
        return value;
}
