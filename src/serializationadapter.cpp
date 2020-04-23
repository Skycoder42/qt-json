#include "serializationadapter.h"
#include "serializablewrapper.h"
#include "qtjson_metamacros.h"
using namespace QtJson;

QReadWriteLock SerializationAdapter::wrapperFactoriesLock;
QHash<int, ISerializableWrapperFactory*> SerializationAdapter::wrapperFactories {
    {QMetaType::QByteArray, new SerializableWrapper<QByteArray>::Factory{}},
    {QMetaType::QDateTime, new SerializableWrapper<QDateTime>::Factory{}},
};



void SerializationAdapter::registerWrapperFactory(int typeId, ISerializableWrapperFactory *factory)
{
    QWriteLocker lock{&wrapperFactoriesLock};
    if (const auto it = wrapperFactories.find(typeId); it != wrapperFactories.end()) {
        delete *it;
        *it = factory;
    } else
        wrapperFactories.insert(typeId, factory);
}

SerializableContainer SerializationAdapter::obtainSerializable(int typeId, QVariant &variant)
{
    // assure variant is instance of property type
    if (variant.userType() != typeId &&
        !variant.convert(typeId)) {
        return nullptr;
    }

    QReadLocker lock{&wrapperFactoriesLock};
    const auto wrapper = wrapperFactories.value(typeId);
    if (wrapper)
        return {wrapper->createWrapper(variant.data()), true};
    else
        return nullptr;
}

SerializableContainer SerializationAdapter::obtainSerializable(const void *gadget, const QMetaObject *metaObject, const QMetaProperty &metaProperty, QVariant &variant)
{
    // assure variant is instance of property type
    if (variant.userType() != metaProperty.userType() &&
        !variant.convert(metaProperty.userType())) {
        return nullptr;
    }

    // check if annotated
    const auto mIdx = metaObject->indexOfMethod(serializablePropInfoName(metaProperty));
    if (mIdx >= 0) {
        const auto method = metaObject->method(mIdx);
        ISerializable *serializable = nullptr;
        if (method.invokeOnGadget(const_cast<void *>(gadget),
                                  Q_ARG(void*, variant.data()),
                                  Q_RETURN_ARG(QtJson::ISerializable*, serializable))) {
            if (serializable)
                return {serializable, false};
        }
    }

    return obtainSerializable(metaProperty.userType(), variant);
}

QByteArray SerializationAdapter::serializablePropInfoName(const QMetaProperty &property)
{
    const auto rawName = QByteArrayLiteral(QTJSON_SERIALIZABLE_PROP_KEY_STR) +
                         property.name() +
                         QByteArrayLiteral("()");
    return QMetaObject::normalizedSignature(rawName);
}



SerializableContainer::SerializableContainer(std::nullptr_t) {}

SerializableContainer::SerializableContainer(ISerializable *serializable, bool cleanup) :
    _serializable{serializable},
    _cleanup{cleanup}
{}

SerializableContainer::SerializableContainer(SerializableContainer &&other) noexcept
{
    qSwap(_serializable, other._serializable);
    qSwap(_cleanup, other._cleanup);
}

SerializableContainer &SerializableContainer::operator=(SerializableContainer &&other) noexcept
{
    qSwap(_serializable, other._serializable);
    qSwap(_cleanup, other._cleanup);
    return *this;
}

SerializableContainer::~SerializableContainer()
{
    if (_cleanup && _serializable)
        delete _serializable;
}

QtJson::SerializableContainer::operator bool() const
{
    return _serializable;
}

bool SerializableContainer::operator!() const
{
    return !_serializable;
}

ISerializable *SerializableContainer::operator->() const
{
    return _serializable;
}
