#pragma once

#include "iserializable.h"

namespace QtJson {

class QTJSON_EXPORT ISerializableWrapperFactory
{
    Q_DISABLE_COPY(ISerializableWrapperFactory)
public:
    ISerializableWrapperFactory();
    virtual ~ISerializableWrapperFactory();

    virtual ISerializable *createReadWrapper(const void *data) const = 0;
    virtual ISerializable *createWriteWrapper(void *data) const = 0;
};

template <typename TType>
class ITypedSerializableWrapperFactory : public ISerializableWrapperFactory
{
public:
    virtual ISerializable *createReadWrapper(const TType *data) const = 0;
    virtual ISerializable *createWriteWrapper(TType *data) const = 0;

    inline ISerializable *createReadWrapper(const void *data) const final {
        createReadWrapper(reinterpret_cast<const TType*>(data));
    }
    inline ISerializable *createWriteWrapper(void *data) const final {
        createReadWrapper(reinterpret_cast<TType*>(data));
    }
};

}

