#include "qtjson.h"
#include <QtCore/QSet>
#include <QtCore/QMetaProperty>
#include <QtCore/QSharedPointer>
#include <QtCore/QPointer>
#include <QtCore/QSequentialIterable>
#include <QtCore/QAssociativeIterable>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <optional>
using namespace QtJson;

namespace {

const QSet<int> MetaExceptions {
    QMetaType::QKeySequence,
    QMetaType::QFont,
    QMetaType::QLocale,
};

QJsonObject mapGadget(const QMetaObject *mo, const void *gadget, Configuration configuration) {
    QJsonObject jObj;
    for (auto i = 0; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (!property.isStored())
            continue;
        jObj.insert(QString::fromUtf8(property.name()),
                    QtJson::stringify(property.readOnGadget(gadget), configuration));
    }
    return jObj;
}

QJsonObject mapObject(QObject *object, Configuration configuration) {
    QJsonObject jObj;
    const auto mo = object->metaObject();
    for (auto i = configuration.keepObjectName ? 0 : 1; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (!property.isStored())
            continue;
        jObj.insert(QString::fromUtf8(property.name()),
                    QtJson::stringify(property.read(object), configuration));
    }
    return jObj;
}

}

QJsonValue QtJson::stringify(const QVariant &value, Configuration configuration)
{
    if (!value.isValid())
        return QJsonValue::Undefined;
    if (value.isNull())
        return QJsonValue::Null;

    // check for objects/gadgets
    const auto type = value.userType();
    const auto mo = QMetaType::metaObjectForType(type);
    if (mo && !MetaExceptions.contains(type)) {
        const auto flags = QMetaType::typeFlags(type);
        if (flags.testFlag(QMetaType::PointerToGadget)) {
            Q_ASSERT_X(value.constData(), Q_FUNC_INFO, "value.constData() should not be null");
            const auto gadgetPtr = *reinterpret_cast<const void* const *>(value.constData());
            if (gadgetPtr)
                return mapGadget(mo, gadgetPtr, configuration);
            else
                return QJsonValue::Null;
        } else if (flags.testFlag(QMetaType::IsGadget)) {
            return mapGadget(mo, value.constData(), configuration);
        } else if (flags.testFlag(QMetaType::SharedPointerToQObject)) {
            const auto objPtr = value.value<QSharedPointer<QObject>>();
            if (objPtr)
                return mapObject(objPtr.get(), configuration);
            else
                return QJsonValue::Null;
        } else if (flags.testFlag(QMetaType::WeakPointerToQObject)) {
            const auto weakPtr = value.value<QWeakPointer<QObject>>();
            if (const auto objPtr = weakPtr.toStrongRef(); objPtr)
                return mapObject(objPtr.get(), configuration);
            else
                return QJsonValue::Null;
        } else if (flags.testFlag(QMetaType::TrackingPointerToQObject)) {
            const auto objPtr = value.value<QPointer<QObject>>();
            if (objPtr)
                return mapObject(objPtr, configuration);
            else
                return QJsonValue::Null;
        } else if (flags.testFlag(QMetaType::PointerToQObject)) {
            const auto objPtr = value.value<QObject*>();
            if (objPtr)
                return mapObject(objPtr, configuration);
            else
                return QJsonValue::Null;
        } else if (flags.testFlag(QMetaType::IsEnumeration)) {
            if (configuration.enumAsString) {
                const auto enumName = QString::fromUtf8(QMetaType::typeName(type))
                                          .split(QStringLiteral("::"))
                                          .last()
                                          .toUtf8();
                auto meIndex = mo->indexOfEnumerator(enumName.data());
                if (meIndex >= 0) {
                    const auto me = mo->enumerator(meIndex);
                    if (me.isFlag())
                        return QString::fromUtf8(me.valueToKeys(value.toInt()));
                    else
                        return QString::fromUtf8(me.valueToKey(value.toInt()));
                }
            } else
                return value.toInt();
        }
    }

    // check for lists
    if (value.canConvert(QMetaType::QVariantList)) {
        QJsonArray jList;
        for (const auto element : value.value<QSequentialIterable>())
            jList.append(stringify(element, configuration));
        return jList;
    }

    // check for maps
    if (value.canConvert(QMetaType::QVariantMap) ||
        value.canConvert(QMetaType::QVariantHash)) {
        const auto iterator = value.value<QAssociativeIterable>();
        QJsonObject jMap;
        for (auto it = iterator.begin(), end = iterator.end(); it != end; ++it)
            jMap.insert(it.key().toString(), stringify(it.value(), configuration));
        return jMap;
    }

    // all other cases: default convert
    return QJsonValue::fromVariant(value);
}
