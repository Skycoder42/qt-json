#pragma once

#include <variant>
#include <optional>
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



template <typename... TSerializables>
class SerializationMultiTest : public SerializationTestBase
{
public:
    using Variant = std::variant<QSharedPointer<const TSerializables>...>;

protected:
    void compare(const ConstSerPtr &lhs, const ConstSerPtr &rhs, const char *file, int line) const override;
    SerPtr cloneEmpty(const ConstSerPtr &base) override;

    inline std::optional<Variant> extractVariant(const ConstSerPtr &data) const;

private:
    template <typename TFirst>
    inline std::optional<Variant> extract(const ConstSerPtr &data) const;
    template <typename TFirst, typename TSecond, typename... TArgs>
    inline std::optional<Variant> extract(const ConstSerPtr &data) const;
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



template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::compare(const SerializationTestBase::ConstSerPtr &lhs, const SerializationTestBase::ConstSerPtr &rhs, const char *file, int line) const
{
    using ValVariant = std::variant<TSerializables...>;
    const auto toValVariant = [](const auto &data) -> ValVariant {
        return *data;
    };
    const auto toString = [](const auto &data) -> QString {
        return QVariant::fromValue(data).toString();
    };

    const auto lhsVar = extractVariant(lhs);
    QVERIFY(lhsVar);
    const auto rhsVar = extractVariant(rhs);
    QVERIFY(rhsVar);
    const auto lhsData = std::visit(toValVariant, *lhsVar);
    const auto rhsData = std::visit(toValVariant, *rhsVar);
    const auto lStr = std::visit(toString, lhsData);
    const auto rStr = std::visit(toString, rhsData);
    if (!QTest::qCompare(lhsData, rhsData, qUtf8Printable(lStr), qUtf8Printable(rStr), file, line)){
        qCritical().noquote() << "Actual:  " << lStr;
        qCritical().noquote() << "Expected:" << rStr;
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
