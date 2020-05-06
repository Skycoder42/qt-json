#include "qtjson_common_p.h"
#include "qtjson_exception.h"

QVariant QtJson::__private::findInfo(const QMetaObject *metaObject, const char *key, const QVariant &defaultValue)
{
	const auto cIdx = metaObject->indexOfClassInfo(key);
	if (cIdx < 0)
		return defaultValue;
	else
		return QVariant{QString::fromUtf8(metaObject->classInfo(cIdx).value())};
}

void QtJson::__private::verifyTagImpl(QCborTag tag, bool allowUntagged, const QList<QCborTag> &expected)
{
	if (tag == static_cast<QCborTag>(-1) && allowUntagged)
		return;
	if (expected.isEmpty() && tag != static_cast<QCborTag>(-1))
		return;
	if (expected.contains(tag))
		return;
	throw InvalidValueTagException{tag, expected};
}
