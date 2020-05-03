#pragma once

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

	void compare(const QJsonValue &lhs, const QJsonValue &rhs, const char *file, int line) const;
	void compare(const QCborValue &lhs, const QCborValue &rhs, const char *file, int line) const;
	virtual void compare(const ConstSerPtr &lhs, const ConstSerPtr &rhs, const char *file, int line) const = 0;

	QByteArray stringiy(const QJsonValue &value) const;

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
	void compare(const ConstSerPtr &lhs, const ConstSerPtr &rhs, const char *file, int line) const override;

	SerPtr cloneEmpty(const ConstSerPtr &base) override;

	template <typename... TArgs>
	ConstSerPtr d(TArgs&& ...data) const {
		return QSharedPointer<const TSerializable>::create(std::forward<TArgs>(data)...);
	}
};



template<typename TSerializable>
void SerializationTest<TSerializable>::compare(const SerializationTestBase::ConstSerPtr &lhs, const SerializationTestBase::ConstSerPtr &rhs, const char *file, int line) const
{
	QVERIFY(lhs.dynamicCast<const TSerializable>());
	QVERIFY(rhs.dynamicCast<const TSerializable>());
	const auto &lhsData = *lhs.staticCast<const TSerializable>();
	const auto &rhsData = *rhs.staticCast<const TSerializable>();
	const auto lStr = QVariant{lhsData}.toString();
	const auto rStr = QVariant{rhsData}.toString();
	if (!QTest::qCompare(lhsData, rhsData, qUtf8Printable(lStr), qUtf8Printable(rStr), file, line)){
		qCritical().noquote() << "Actual:  " << lStr;
		qCritical().noquote() << "Expected:" << rStr;
	}
}

template<typename TSerializable>
SerializationTestBase::SerPtr SerializationTest<TSerializable>::cloneEmpty(const ConstSerPtr &base)
{
	if (!QTest::qVerify(base.dynamicCast<const TSerializable>(), "base.dynamicCast<TSerializable>()", "", __FILE__, __LINE__))\
		return {};
	return QSharedPointer<TSerializable>::create();
}
