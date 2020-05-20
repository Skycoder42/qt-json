# qt-json
A basic package for simple JSON/CBOR serialization between JSON/CBOR and qt classes

## Features
- Can serialize many Qt-classes out of the box:
    - QList, QVector, QQueue, QStack, QLinkedList, QSet
    - QMap, QHash
    - QByteArray
    - QDateTime
    - QVersionNumber
    - std::optional
- Works recursively
- Easily extendable to support any C++-class via specialization
- Provides a serializable interface for defining serialization-enabled types
- Provides macros to auto-generate serialization for any Q_GADGET

## Installation
The package is provided via qdep, as `Skycoder42/qt-json`. To use it simply:

1. Install and enable qdep (See [qdep - Installing](https://github.com/Skycoder42/qdep#installation))
2. Add the following to your pro-file:

```qmake
QDEP_DEPENDS += Skycoder42/qt-json
!load(qdep):error("Failed to load qdep feature! Run 'qdep.py prfgen --qmake $$QMAKE_QMAKE' to create it.")
```

### Alternative Installation
If you don't want to use qdep, you can simply clone the project instead (for example, add it as git submodule) and
add it to your project via:

```qmake
include(path/to/qt-json/qt-json.pri)
```

**NOTE:** The package will work normally either ways, but not using qdep makes it harder to manage recursive
dependencies, or to export the package from within a shared library.

## Usage
Usage of the library is very straight-forward. Here a basic example, that serializes a list of QDateTimes:

```c++
#include <QtCore>
#include <qt-json.h>

int main() {
    QList<QDateTime> data {
        QDateTime::currentDateTime(),
        QDateTime::currentDateTime().addDays(1),
        QDateTime::currentDateTime().addDays(2),
    };
    qDebug().noquote() << QtJson::stringify(data); // prints out the JSON as a QString

    QString text = "...";
    const auto newData = QtJson::parseString<QList<QDateTime>>(text); // parses JSON string to C++

    return 0;
}
```

### Custom serializable gadgets
Defining custom gadgets with auto-generated serialization can be done as well. The following sample code demonstrates
how:

```c++
#include <qt-json.h>

class MyGadget : public QtJson::SerializableGadget
{
    Q_GADGET
    QTJSON_SERIALIZABLE_GADGET(MyGadget)

    // Simple types (int, bool, QString, ...) can simply be declared as normal property
    Q_PROPERTY(int id MEMBER id USER true)

    // Properties with STORED = false are ignored and are not de/serialized
    Q_PROPERTY(bool valid READ isValid STORED false);

    // Enum and Flags-Support is built in, too
    Q_PROPERTY(MyEnum myEnum MEMBER myEnum)

    // More complex types (QList, QDateTime, ISerializables, ...) need to be declared as special property
    // This creates a member "elements" and a property with the same name
    QTJSON_PROP(elements, QList<QByteArray>) = {"42"};  // initializer is optional

    // If you need more control, you can split that up into the property + extra
    Q_PROPERTY(double special READ special WRITE setSpecial)
    QTJSON_SERIALIZABLE_PROP(special, special, setSpecial, double)

    // also works for member props
    Q_PROPERTY(bool something MEMBER _something)
    QTJSON_SERIALIZABLE_PROP_MEMBER(something, _something, bool)

public:
    enum MyEnum {
        A,
        B
    };
    Q_ENUM(MyEnum)

    // ...
};
```

The resulting gadget implements ISerializable and handles de/serialization completely. It converts to an object with
each property beeing one element of that object. Normal Q_PROPERTYs are converted using the from/toVariant method
(see TODO) with the exception of enums. For any type that either implements ISerializable or has to be serialized via
the SerializableAdapter (link TODO), QTJSON_PROP (or QTJSON_SERIALIZABLE_PROP[_MEMBER]) must be used.

## Documentation
