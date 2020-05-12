#pragma once

#include "iserializable.h"
#include "serializableadapter.h"

#include <type_traits>

#include <QtCore/QObject>

namespace QtJson {

namespace __private {

enum class AdapterMethod {
	ToJson,
	FromJson,
	ToCbor,
	FromCbor
};

template <typename TType>
struct DataValueInfo;

}

class QTJSON_EXPORT SerializableGadget : public ISerializable
{
	Q_GADGET

public:
	Q_INVOKABLE QJsonValue toJson(const QtJson::Configuration &config = {}) const override;
	Q_INVOKABLE void assignJson(const QJsonValue &value, const QtJson::Configuration &config = {}) override;
	Q_INVOKABLE QCborValue toCbor(const QtJson::Configuration &config = {}) const override;
	Q_INVOKABLE void assignCbor(const QCborValue &value, const QtJson::Configuration &config = {}) override;

protected:
	virtual const QMetaObject *getMetaObject() const = 0;

private:
	static QByteArray serializablePropInfoName(const QMetaProperty &property);
	ISerializable *asSerializable(const QMetaObject *mo,
								  const QMetaProperty &property) const;
	std::optional<QMetaMethod> findMethod(const QMetaObject *mo,
										  const QMetaProperty &property,
										  __private::AdapterMethod method) const;

	template <typename TValue>
	TValue findInfo(const QMetaObject *metaObject, const char *key, const TValue &defaultValue) const;
	template <typename TValue>
	typename __private::DataValueInfo<TValue>::Map serialize(const Configuration &config) const;
	template <typename TValue>
	void deserialize(const typename __private::DataValueInfo<TValue>::Map &map, const Configuration &config);
};

}

#define QTJSON_SERIALIZABLE_KEY "__qtjson_serializable"
#define QTJSON_SERIALIZABLE Q_CLASSINFO(QTJSON_SERIALIZABLE_KEY, "true")

#define QTJSON_SERIALIZABLE_GADGET(Type) \
	QTJSON_SERIALIZABLE \
	public: \
		static Type fromJson(const QJsonValue &value, const QtJson::Configuration &config = {}) { \
			Type _instance; \
			_instance.assignJson(value, config); \
			return _instance; \
		} \
		\
		static Type fromCbor(const QCborValue &value, const QtJson::Configuration &config = {}) { \
			Type _instance; \
			_instance.assignCbor(value, config); \
			return _instance; \
		} \
		\
	protected: \
		inline const QMetaObject *getMetaObject() const override { \
			return &Type::staticMetaObject; \
		} \
		\
	private:

#define QTJSON_PROPERTY_OFFSET_KEY "__qtjson_offset"
#define QTJSON_PROPERTY_OFFSET(offset) Q_CLASSINFO(QTJSON_PROPERTY_OFFSET_KEY, #offset)

#define QTJSON_SERIALIZABLE_PROP(name, get, set, ...) \
	Q_INVOKABLE inline QJsonValue __qtjson_ ## name ## _toJson(const QtJson::Configuration &config) const { \
		return QtJson::SerializableAdapter<__VA_ARGS>::toJson(get(), config); \
	} \
	Q_INVOKABLE inline void __qtjson_ ## name ## _fromJson(const QJsonValue &value, const QtJson::Configuration &config) { \
		set(QtJson::SerializableAdapter<__VA_ARGS>::fromJson(value, config)); \
	} \
	Q_INVOKABLE inline QCborValue __qtjson_ ## name ## _toCbor(const QtJson::Configuration &config) const { \
		return QtJson::SerializableAdapter<__VA_ARGS>::toCbor(get(), config); \
	} \
	Q_INVOKABLE inline void __qtjson_ ## name ## _fromCbor(const QCborValue &value, const QtJson::Configuration &config) { \
		set(QtJson::SerializableAdapter<__VA_ARGS>::fromCbor(value, config)); \
	}

#define QTJSON_SERIALIZABLE_PROP_MEMBER(name, member, ...) \
	QTJSON_SERIALIZABLE_PROP(name, \
							 [this]() { return member; }, \
							 [this](__VA_ARGS__ &&value) { member = std::move(value); }, \
							 __VA_ARGS__)

#define QTJSON_PROP(name, ...) \
	private: \
		Q_PROPERTY(__VA_ARGS__ name MEMBER name STORED true) \
		QTJSON_SERIALIZABLE_PROP_MEMBER(name, name, __VA_ARGS__) \
	public: \
		__VA_ARGS__ name
