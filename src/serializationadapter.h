#pragma once

#include "iserializable.h"
#include "iserializablewrapperfactory.h"

#include <QtCore/QReadWriteLock>
#include <QtCore/QHash>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

namespace QtJson {

struct QTJSON_EXPORT SerializableContainer
{
    Q_DISABLE_COPY(SerializableContainer)
public:
    SerializableContainer(std::nullptr_t);
    SerializableContainer(ISerializable *serializable, bool cleanup);
    SerializableContainer(SerializableContainer &&other) noexcept;
    SerializableContainer &operator=(SerializableContainer &&other) noexcept;
    ~SerializableContainer();

    explicit operator bool() const;
    bool operator!() const;

    ISerializable *operator->() const;

private:
    ISerializable *_serializable = nullptr;
    bool _cleanup = false;
};

class QTJSON_EXPORT SerializationAdapter
{
public:
    static void registerWrapperFactory(int typeId, ISerializableWrapperFactory *factory);
    template <typename TType>
    inline void registerWrapperFactory(ITypedSerializableWrapperFactory<TType> *factory) {
        registerWrapperFactory(qMetaTypeId<TType>(), factory);
    }

    static SerializableContainer obtainSerializable(int typeId, QVariant &variant);
    template <typename TType>
    static inline SerializableContainer obtainSerializable(QVariant &variant) {
        return obtainSerializable(qMetaTypeId<TType>(), variant);
    }

    static SerializableContainer obtainSerializable(const void *gadget,
                                                    const QMetaObject *metaObject,
                                                    const QMetaProperty &metaProperty,
                                                    QVariant &variant);
    template <typename TGadget>
    static inline SerializableContainer obtainSerializable(const TGadget *gadget,
                                                           const QMetaProperty &metaProperty,
                                                           QVariant &variant) {
        return obtainSerializable(gadget, &TGadget::staticMetaObject, metaProperty, variant);
    }

private:
    static QReadWriteLock wrapperFactoriesLock;
    static QHash<int, ISerializableWrapperFactory*> wrapperFactories;

    static QByteArray serializablePropInfoName(const QMetaProperty &property);
};

template <typename TType, typename = void>
class SerializableAdapter;

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<std::is_base_of_v<ISerializable, TType>, void>>
{
public:
    QJsonValue toJson(const JsonConfiguration &config = {}) const;
    void assignJson(const QJsonValue &value, const JsonConfiguration &config = {});

    QCborValue toCbor(const CborConfiguration &config = {}) const;
    void assignCbor(const QCborValue &value, const CborConfiguration &config = {});
};

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<!std::is_base_of_v<ISerializable, TType>, void>>
{
public:
    QJsonValue toJson(const JsonConfiguration &config = {}) const;
    void assignJson(const QJsonValue &value, const JsonConfiguration &config = {});

    QCborValue toCbor(const CborConfiguration &config = {}) const;
    void assignCbor(const QCborValue &value, const CborConfiguration &config = {});
};

}
