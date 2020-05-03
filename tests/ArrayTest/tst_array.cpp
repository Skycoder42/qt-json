#include <QtTest>
#include <serializationtest.h>
#include <serializablearray.h>
using namespace QtJson;

class ArrayTest : public SerializationMultiTest<
                                  SerializableList<int>,
                                  SerializableVector<int>>
{
    Q_OBJECT

protected:
    void setupData() const override;
    void setupSerData() const override;
    void setupDeserData() const override;
};

void ArrayTest::setupData() const
{

}

void ArrayTest::setupSerData() const
{

}

void ArrayTest::setupDeserData() const
{

}

QTEST_APPLESS_MAIN(ArrayTest)

#include "tst_array.moc"
