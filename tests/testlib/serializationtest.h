#pragma once

#include <variant>
#include <optional>
#include <type_traits>
#include <QtTest>

#include "iserializable.h"
#include "qtjson_exception.h"

Q_DECLARE_METATYPE(QSharedPointer<const QtJson::ISerializable>)

class SerializationTestBase : public QObject
{
	Q_OBJECT

public:
	using SerPtr = QSharedPointer<QtJson::ISerializable>;
	using ConstSerPtr = QSharedPointer<const QtJson::ISerializable>;

protected:
	virtual void setupData() const;
	virtual void setupSerData() const;
	virtual void setupDeserData() const;

	void compare(const QJsonValue &actual, const QJsonValue &expected, const char *file, int line) const;
	void compare(const QCborValue &actual, const QCborValue &expected, const char *file, int line) const;
	virtual void compare(const ConstSerPtr &actual, const ConstSerPtr &expected, const char *file, int line) const = 0;

	QByteArray stringify(const QJsonValue &value) const;

	virtual SerPtr cloneEmpty(const ConstSerPtr &base) = 0;

	template <typename TSerializable, typename... TArgs>
	ConstSerPtr d(TArgs&& ...data) const {
		return QSharedPointer<const TSerializable>::create(std::forward<TArgs>(data)...);
	}

private Q_SLOTS:
	void testSerialization_data();
	void testSerialization();
	void testDeserialization_data();
	void testDeserialization();
};



template <typename TSerializable>
class SerializationTest : public SerializationTestBase
{
protected:
	void compare(const ConstSerPtr &actual, const ConstSerPtr &expected, const char *file, int line) const override;

	SerPtr cloneEmpty(const ConstSerPtr &base) override;

	template <typename... TArgs>
	ConstSerPtr d(TArgs&& ...data) const {
		return QSharedPointer<const TSerializable>::create(std::forward<TArgs>(data)...);
	}
};



template <typename... TSerializables>
class SerializationMultiTest : public SerializationTestBase
{
public:
	using Variant = std::variant<QSharedPointer<const TSerializables>...>;

protected:
	void compare(const ConstSerPtr &actual, const ConstSerPtr &expected, const char *file, int line) const override;
	SerPtr cloneEmpty(const ConstSerPtr &base) override;

	inline std::optional<Variant> extractVariant(const ConstSerPtr &data) const;

	template <typename TSerializable, typename... TArgs>
	ConstSerPtr d(TArgs&& ...data) const {
		static_assert (std::disjunction_v<std::is_same<TSerializable, TSerializables>...>, "TSerializable must be one of the TSerializables");
		return QSharedPointer<const TSerializable>::create(std::forward<TArgs>(data)...);
	}

private:
	template <typename TFirst>
	inline std::optional<Variant> extract(const ConstSerPtr &data) const;
	template <typename TFirst, typename TSecond, typename... TArgs>
	inline std::optional<Variant> extract(const ConstSerPtr &data) const;
};



template<typename TSerializable>
void SerializationTest<TSerializable>::compare(const SerializationTestBase::ConstSerPtr &actual, const SerializationTestBase::ConstSerPtr &expected, const char *file, int line) const
{
	QVERIFY(actual.dynamicCast<const TSerializable>());
	QVERIFY(expected.dynamicCast<const TSerializable>());
	const auto &aData = *actual.staticCast<const TSerializable>();
	const auto &eData = *expected.staticCast<const TSerializable>();
	if (!QTest::qCompare(aData, eData, "actual", "expected", file, line)){
		qCritical().noquote() << "Actual:  " << aData;
		qCritical().noquote() << "Expected:" << eData;
	}
}

template<typename TSerializable>
SerializationTestBase::SerPtr SerializationTest<TSerializable>::cloneEmpty(const ConstSerPtr &base)
{
	if (!QTest::qVerify(base.dynamicCast<const TSerializable>(), "base.dynamicCast<TSerializable>()", "", __FILE__, __LINE__))\
		return {};
	return QSharedPointer<TSerializable>::create();
}



template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::compare(const SerializationTestBase::ConstSerPtr &lhs, const SerializationTestBase::ConstSerPtr &rhs, const char *file, int line) const
{
	using ValVariant = std::variant<TSerializables...>;
	const auto toValVar = [](const auto &data) -> ValVariant {
		return *data;
	};

	const auto actualVar = extractVariant(lhs);
	QVERIFY(actualVar);
	const auto expectedVar = extractVariant(rhs);
	QVERIFY(expectedVar);
	const auto actualData = std::visit(toValVar, *actualVar);
	const auto expectedData = std::visit(toValVar, *expectedVar);
	if (!QTest::qCompare(actualData, expectedData, "actual", "expected", file, line)){
		std::visit([](const auto &data) {
			qCritical().noquote() << "Actual:  " << data;
		}, actualData);
		std::visit([](const auto &data) {
			qCritical().noquote() << "Expected:" << data;
		}, expectedData);
	}
}

template<typename... TSerializables>
SerializationTestBase::SerPtr SerializationMultiTest<TSerializables...>::cloneEmpty(const SerializationTestBase::ConstSerPtr &base)
{
	const auto var = extractVariant(base);
	if (!QTest::qVerify(static_cast<bool>(var), "extractVariant(base)", "", __FILE__, __LINE__))\
		return {};
	return std::visit([](const auto &data) -> SerPtr {
		return QSharedPointer<std::decay_t<decltype(*data)>>::create();
	}, *var);
}

template<typename... TSerializables>
inline std::optional<typename SerializationMultiTest<TSerializables...>::Variant> SerializationMultiTest<TSerializables...>::extractVariant(const SerializationTestBase::ConstSerPtr &data) const
{
	return extract<TSerializables...>(data);
}

template<typename... TSerializables>
template <typename TFirst>
inline std::optional<typename SerializationMultiTest<TSerializables...>::Variant> SerializationMultiTest<TSerializables...>::extract(const ConstSerPtr &data) const
{
	if (const auto d = data.dynamicCast<const TFirst>(); d)
		return d;
	else
		return std::nullopt;
}

template<typename... TSerializables>
template <typename TFirst, typename TSecond, typename... TArgs>
inline std::optional<typename SerializationMultiTest<TSerializables...>::Variant> SerializationMultiTest<TSerializables...>::extract(const ConstSerPtr &data) const
{
	if (const auto d = data.dynamicCast<const TFirst>(); d)
		return d;
	else
		return extract<TSecond, TArgs...>(data);
}
