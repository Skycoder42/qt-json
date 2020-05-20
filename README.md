# qt-json
A basic package for simple JSON/CBOR serialization between JSON/CBOR and qt classes

* [Features](#features)
* [Installation](#installation)
    + [Alternative Installation](#alternative-installation)
* [Usage](#usage)
    + [Custom serializable gadgets](#custom-serializable-gadgets)
* [Documentation](#documentation)
    + [QtJson namespace](#qtjson-namespace)
        - [Configuration](#configuration)
    + [ISerializable](#iserializable)
        - [Custom ISerializable example](#custom-iserializable-example)
    + [SerializableAdapter](#serializableadapter)
        - [Custom Adapter example](#custom-adapter-example)
    + [SerializableGadget](#serializablegadget)
        - [Enums and Flags](#enums-and-flags)
    + [Fallback conversion](#fallback-conversion)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>


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
- Global extendable configuration to make it possible to create configurable de/serializers
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
(see [Fallback conversion](#fallback-conversion)) with the exception of enums. For any type that either implements
ISerializable or has to be serialized via the [SerializableAdapter](#serializableadapter), QTJSON_PROP (or
QTJSON_SERIALIZABLE_PROP[_MEMBER]) must be used.

## Documentation
The package does not expose many APIs, but defines a few concepts that you need to follow when adding your own
serializable types. The following documentation goes into detail in regards of those points.

As a broader overview, here are the parts of the package that are of interest:

- **QtJson namespace:** Provides a set of globals as the Consumer-API
- **ISerializable:** A basic interface that can be implemented for custom types
- **SerializableAdapter:** A generic calls that can add serialization to existing types via template specialization
- **Fallback conversion:** What happens if no interface or adapter implementation apply to a type

### QtJson namespace
The public namespace, defined in the `qt-json.h` header, provides three groups of functions:

- **read/writeJson/Cbor:** Reads and writes QJsonValue/QCborValue from and to their string or binary representation.
These methods simply exist for convenience and internally use QJsonDocument and QCborValue directly to perform the
conversion
- **from/toJson/Cbor:** Perform the actual conversions between C++ classes to QJsonValue/QCborValue. This is where the
main part of the library operates and where ISerializable and the SerializableAdapter are used.
- **stringify/binarify/parse*:** Combines the previous two function groups to allow direct conversions between C++ and
the binary or string data

#### Configuration
It is possible to pass a Configuration object to all of the serializer functions to control how the package should
operate. The following table lists all configuration options with defaults and a short explanation:

 Option          | Type            | Default               | Direction | Description
-----------------|-----------------|-----------------------|-----------|-------------
 ignoreStored    | bool            | false                 | both      | If true, properties with "STORED true" are still serialized. Otherwise, they are skipped
 byteArrayMode   | ByteArrayMode   | ByteArrayMode::Base64 | out       | How to represent bytearrays in JSON. Can be base64, base64url or hex encoding
 dateAsTimeStamp | bool            | false                 | out       | If true, QDateTime is serialized as unix timestamp instead of an ISO string
 enumAsString    | bool            | true                  | out       | If true, enums are serialized as strings. If false, they are written as integers
 versionAsString | bool            | true                  | out       | If true, version numbers are writtes as strings. If false, they become an array of integers
 validation      | ValidationFlags | ValidationFlag::Full  | in        | Specifies how strict gadget deserialization should be. You can either allow or disallow both missing and extra members
 extraConfig     | QVariantMap     | _<empty>_             |           | Additional config for your own types

### ISerializable
The ISerializable is a basic interface which should be used to add serialization capabilities to types that you
implement. It declares methods to convert the type from and to json and cbor. Any type implementing this interface will
automatically be detected as serializable by the adapter and the global functions and work everywhere, without any
additional declarations needed.

#### Custom ISerializable example
A basic example for a plain, simple custom ISerializable type:

```cpp
struct MyType : public QtJson::ISerializable
{
    int data = 0;

    inline QJsonValue toJson(const Configuration &config = {}) const override {
        return data;
    }

    inline void assignJson(const QJsonValue &value, const Configuration &config = {}) override {
        data = value.toInt();
    }

    inline QCborValue toCbor(const Configuration &config = {}) const override {
        return data;
    }

    inline void assignCbor(const QCborValue &value, const Configuration &config = {}) override {
        data = static_cast<int>(value.toInteger());
    }
};
```

### SerializableAdapter
The adapter is a static, generic class that can be specialized to provider converters from and to json and cbor for any
type. It should be used in cases where you have an existing type that does not implement ISerializable and cannot be
modified (like Qt-classes or other external stuff). It provides the following generic interface:

```cpp
template <typename TType, typename = void>
class SerializableAdapter
{
public:
    static inline QJsonValue toJson(const TType &value, const Configuration &config = {});
    static inline TType fromJson(const QJsonValue &value, const Configuration &config = {});
    static inline QCborValue toCbor(const TType &value, const Configuration &config = {});
    static inline TType fromCbor(const QCborValue &value, const Configuration &config = {});
};
```

It has two template parameters to support SFINAE based partial specialization, but the second parameter is defaulted
and can be left out when using the adapter. Typically, you will not use the adapter directly, unless you want to
implement your own adapter that needs to serialized child elements. In the following section, an example will be used to
demonstrate how to extend a type.

When using the adapter to de/serialize something, is is resolved as follows:

- If `TType` implements ISerializable, the the from/toJson/Cbor methods of the interface are used
- If a specialization matches `TType`, than that is used
- If neither is the case, the [standard variant conversion](#fallback-conversion) is used

#### Custom Adapter example
The following adapter implements serialization for any `std::vector` instance. It serializes the vector to a JSON or
CBOR array and back. In this example, we use a partial specialization of adapter, in order to be able to support any
vector, regardless of the actual type. Furthermore, we use the adapter inside of the implementation to be able to
correctly serialize any T, no matter whether it actually has an adapter or not. (See explanation above).

```cpp
namespace QtJson {

template <typename T, typename Allocator = std::allocator<T>>
class SerializableAdapter<std::vector<T, Allocator>, void>
{
public:
    using vector_type = std::vector<T, Allocator>;

    static inline QJsonValue toJson(const vector_type &value, const Configuration &config = {}) {
        QJsonArray jArray;
        for (const auto &element : value)
            jArray.append(SerializableAdapter<T>::toJson(element, config));
        return jArray;
    }

    static inline vector_type fromJson(const QJsonValue &value, const Configuration &config = {}) {
        if (value.type() != QJsonValue::Array)
            throw InvalidValueTypeException{value.type(), {QJsonValue::Array}};
        const auto jArray = value.toArray();
        vector_type vec;
        vec.reserve(jArray.size());
        for (const auto &element : jArray)
            vec.push_back(SerializableAdapter<T>::fromJson(element, config));
        return vec;
    }

    static inline QCborValue toCbor(const vector_type &value, const Configuration &config = {}) {
        QCborArray cArray;
        for (const auto &element : value)
            cArray.append(SerializableAdapter<T>::toCbor(element, config));
        return cArray;
    }

    static inline vector_type fromCbor(const QCborValue &value, const Configuration &config = {}) {
        if (value.type() != QCborValue::Array)
            throw InvalidValueTypeException{value.type(), {QCborValue::Array}};
        const auto cArray = value.toArray();
        vector_type vec;
        vec.reserve(cArray.size());
        for (const auto &element : cArray)
            vec.push_back(SerializableAdapter<T>::fromCbor(element, config));
        return vec;
    }
};

}
```

If you want to use a full specialization, you can do so as well:

```cpp
template <>
class SerializableAdapter<MyType, void>
```

Thanks to the second void parameter, you can also use SFINAE based template specialization. For example, you could
use something similar as shown below to create an adapter that is used for any type that extends `MyBaseClass`:

```cpp
template <typename T>
class SerializableAdapter<T, std::enable_if_t<std::is_base_of<MyBaseClass, T>, void>>
```

**Important:** Be careful to not define your specializations to broad, as that might lead to situations where two or
specializations could handle one type. This is not allowed and leads to compilation errors. In other words: You cannot
"override" existing adapter specializations (But you can simply not include them).

### SerializableGadget
The SerializableGadget is an abstract base class that implements ISerializable and can be used to easily define
custom gadgets that are automatically serializable via their properties. An example of such a gadget can be found in
the [Custom serializable gadgets section](#custom-serializable-gadgets).

The gadget works by iterating over all declared properties and using automatically generated helper functions to perform
the de/serialization. Internally, the SerializableAdapter is used for any property declared via one of the property
macros.

Please make sure to always include all headers of adapters for types that you use, as they need to be included in order
to be used.

#### Enums and Flags
Enums and flags are automatically serialized to their string (or integer) representation, simply by using a normal
Q_PROPERTY. Do *not* use QTJSON_PROP or one of the other special properties, as those will override the standard enum
conversion to use an adapter - which is not defined by default, so the fallback conversion is used.

### Fallback conversion
The fallback conversion is very simple. It is used whenever the type neither implements ISerializable or has an adapter
defined. It is also used for any non-enum/flag property declared as just Q_PROPERTY, without one of the special QTJSON_*
macros. The fallback uses QJsonValue::from/toVariant and QCborValue::from/toVariant. Check the documentation for how
those handle data:

- [QJsonValue::toVariant](https://doc.qt.io/qt-5/qjsonvalue.html#toVariant)
- [QJsonValue::fromVariant](https://doc.qt.io/qt-5/qjsonvalue.html#fromVariant)
- [QCborValue::toVariant](https://doc.qt.io/qt-5/qcborvalue.html#toVariant)
- [QCborValue::fromVariant](https://doc.qt.io/qt-5/qcborvalue.html#fromVariant)
