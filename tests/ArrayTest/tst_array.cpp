#include <QtTest>
#include <serializationtest.h>
#include <serializablearray.h>
#include <testserializable.h>
using namespace QtJson;

class ArrayTest : public SerializationMultiTest<
                      SerializableList<int>,
                      SerializableVector<int>,
#ifndef QT_NO_LINKED_LIST
                      SerializableLinkedList<int>,
#endif
                      SerializableQueue<int>,
                      SerializableStack<int>,
                      SerializableSet<int>,
                      SerializableList<TestSerializable>>
{
    Q_OBJECT

protected:
    void setupData() const override;
    void setupSerData() const override;
    void setupDeserData() const override;

private:
    template <typename... TArgs>
    inline ConstSerPtr dl(TArgs... args) const {
        return d<SerializableList<int>>(SerializableList<int>{args...});
    }
    template <typename... TArgs>
    inline ConstSerPtr dv(TArgs... args) const {
        return d<SerializableVector<int>>(SerializableVector<int>{args...});
    }
#ifndef QT_NO_LINKED_LIST
    template <typename... TArgs>
    inline ConstSerPtr dll(TArgs... args) const {
        return d<SerializableLinkedList<int>>(SerializableLinkedList<int>{args...});
    }
#endif
    template <typename... TArgs>
    inline ConstSerPtr dq(TArgs... args) const {
        SerializableQueue<int> q;
        int x[] = {0, (q.append(args),0)...};
        Q_UNUSED(x);
        return d<SerializableQueue<int>>(q);
    }
    template <typename... TArgs>
    inline ConstSerPtr ds(TArgs... args) const {
        SerializableStack<int> s;
        int x[] = {0, (s.append(args),0)...};
        Q_UNUSED(x);
        return d<SerializableStack<int>>(s);
    }
    template <typename... TArgs>
    inline ConstSerPtr dhs(TArgs... args) const {
        return d<SerializableSet<int>>(SerializableSet<int>{args...});
    }
    template <typename... TArgs>
    inline ConstSerPtr dls(TArgs... args) const {
        return d<SerializableList<TestSerializable>>(SerializableList<TestSerializable>{TestSerializable{args}...});
    }
};

void ArrayTest::setupData() const
{
    QTest::addRow("list.filled") << CommonConfiguration{}
                                 << dl(1, 2, 3)
                                 << QJsonValue{QJsonArray{1, 2, 3}}
                                 << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
                                 << false;
    QTest::addRow("list.empty") << CommonConfiguration{}
                                << dl()
                                << QJsonValue{QJsonArray{}}
                                << QCborValue{HomogeneousArrayTag, QCborArray{}}
                                << false;

    QTest::addRow("vector.filled") << CommonConfiguration{}
                                   << dv(1, 2, 3)
                                   << QJsonValue{QJsonArray{1, 2, 3}}
                                   << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
                                   << false;
    QTest::addRow("vector.empty") << CommonConfiguration{}
                                  << dv()
                                  << QJsonValue{QJsonArray{}}
                                  << QCborValue{HomogeneousArrayTag, QCborArray{}}
                                  << false;

#ifndef QT_NO_LINKED_LIST
    QTest::addRow("linkedlist.filled") << CommonConfiguration{}
                                       << dll(1, 2, 3)
                                       << QJsonValue{QJsonArray{1, 2, 3}}
                                       << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
                                       << false;
    QTest::addRow("linkedlist.empty") << CommonConfiguration{}
                                      << dll()
                                      << QJsonValue{QJsonArray{}}
                                      << QCborValue{HomogeneousArrayTag, QCborArray{}}
                                      << false;
#endif

    QTest::addRow("queue.filled") << CommonConfiguration{}
                                  << dq(1, 2, 3)
                                  << QJsonValue{QJsonArray{1, 2, 3}}
                                  << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
                                  << false;
    QTest::addRow("queue.empty") << CommonConfiguration{}
                                 << dq()
                                 << QJsonValue{QJsonArray{}}
                                 << QCborValue{HomogeneousArrayTag, QCborArray{}}
                                 << false;

    QTest::addRow("stack.filled") << CommonConfiguration{}
                                  << ds(1, 2, 3)
                                  << QJsonValue{QJsonArray{1, 2, 3}}
                                  << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
                                  << false;
    QTest::addRow("stack.empty") << CommonConfiguration{}
                                 << ds()
                                 << QJsonValue{QJsonArray{}}
                                 << QCborValue{HomogeneousArrayTag, QCborArray{}}
                                 << false;

    QTest::addRow("set.empty") << CommonConfiguration{}
                               << dhs()
                               << QJsonValue{QJsonArray{}}
                               << QCborValue{FiniteSetTag, QCborArray{}}
                               << false;

    QTest::addRow("list.serializable.filled") << CommonConfiguration{}
                                              << dls(1.1, 2.2, 3.3)
                                              << QJsonValue{QJsonArray{1.1, 2.2, 3.3}}
                                              << QCborValue{HomogeneousArrayTag, QCborArray{1.1, 2.2, 3.3}}
                                              << false;
    QTest::addRow("list.serializable.empty") << CommonConfiguration{}
                                             << dls()
                                             << QJsonValue{QJsonArray{}}
                                             << QCborValue{HomogeneousArrayTag, QCborArray{}}
                                             << false;
}

void ArrayTest::setupSerData() const
{
    QTest::addRow("set.filled") << CommonConfiguration{}
                                << dhs(1)
                                << QJsonValue{QJsonArray{1}}
                                << QCborValue{FiniteSetTag, QCborArray{1}}
                                << false;
}

void ArrayTest::setupDeserData() const
{
    QTest::addRow("set.filled") << CommonConfiguration{}
                                << dhs(1, 2, 3)
                                << QJsonValue{QJsonArray{1, 2, 3}}
                                << QCborValue{FiniteSetTag, QCborArray{1, 2, 3}}
                                << false;

    QTest::addRow("list.untagged") << CommonConfiguration{}
                                   << dl(1, 2, 3)
                                   << QJsonValue{QJsonValue::Undefined}
                                   << QCborValue{QCborArray{1, 2, 3}}
                                   << false;

    QTest::addRow("vector.untagged") << CommonConfiguration{}
                                     << dv(1, 2, 3)
                                     << QJsonValue{QJsonValue::Undefined}
                                     << QCborValue{QCborArray{1, 2, 3}}
                                     << false;

#ifndef QT_NO_LINKED_LIST
    QTest::addRow("linkedlist.untagged") << CommonConfiguration{}
                                         << dll(1, 2, 3)
                                         << QJsonValue{QJsonArray{1, 2, 3}}
                                         << QCborValue{QCborArray{1, 2, 3}}
                                         << false;
#endif

    QTest::addRow("queue.untagged") << CommonConfiguration{}
                                    << dq(1, 2, 3)
                                    << QJsonValue{QJsonValue::Undefined}
                                    << QCborValue{QCborArray{1, 2, 3}}
                                    << false;

    QTest::addRow("stack.untagged") << CommonConfiguration{}
                                    << ds(1, 2, 3)
                                    << QJsonValue{QJsonValue::Undefined}
                                    << QCborValue{QCborArray{1, 2, 3}}
                                    << false;

    QTest::addRow("set.untagged") << CommonConfiguration{}
                                  << dhs(1, 2, 3)
                                  << QJsonValue{QJsonValue::Undefined}
                                  << QCborValue{QCborArray{1, 2, 3}}
                                  << false;

    QTest::addRow("list.serializable.untagged") << CommonConfiguration{}
                                                << dls(1.1, 2.2, 3.3)
                                                << QJsonValue{QJsonValue::Undefined}
                                                << QCborValue{QCborArray{1.1, 2.2, 3.3}}
                                                << false;
}

QTEST_APPLESS_MAIN(ArrayTest)

#include "tst_array.moc"
