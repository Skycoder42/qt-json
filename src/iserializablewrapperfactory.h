#pragma once

#include "iserializable.h"

namespace QtJson {

class QTJSON_EXPORT ISerializableWrapperFactory
{
	Q_DISABLE_COPY(ISerializableWrapperFactory)
public:
	ISerializableWrapperFactory();
	virtual ~ISerializableWrapperFactory();

	virtual ISerializable *createWrapper(void *data) const = 0;
};

template <typename TType>
class ITypedSerializableWrapperFactory : public ISerializableWrapperFactory
{
public:
	virtual ISerializable *createWrapper(TType *data) const = 0;

	inline ISerializable *createWrapper(void *data) const final {
		return createWrapper(reinterpret_cast<TType*>(data));
	}
};

}

