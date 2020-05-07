#include "testserializable.h"
#include <qtjson_exception.h>
using namespace QtJson;

TestSerializable::TestSerializable() = default;

TestSerializable::TestSerializable(double value, std::optional<QCborKnownTags> tag) :
	value{value},
	tag{tag}
{}

bool TestSerializable::operator==(const TestSerializable &other) const
{
	return value == other.value &&
		tag == other.tag;
}

bool TestSerializable::operator!=(const TestSerializable &other) const
{
	return value != other.value ||
		   tag != other.tag;
}

QJsonValue TestSerializable::toJson(const Configuration &) const
{
	return value;
}

void TestSerializable::assignJson(const QJsonValue &value, const Configuration &)
{
	this->value = value.toDouble();
}

QCborValue TestSerializable::toCbor(const Configuration &) const
{
	return tag ? QCborValue{*tag, value} : value;
}

void TestSerializable::assignCbor(const QCborValue &value, const Configuration &)
{
	if (value.isTag()) {
		tag = static_cast<QCborKnownTags>(value.tag());
		this->value = value.taggedValue().toDouble();
	} else
		this->value = value.toDouble();
}

QDebug operator<<(QDebug debug, const TestSerializable &value)
{
	QDebugStateSaver state{debug};
	debug.noquote().nospace() << "TestSerializable(" << value.value << ")";
	return debug;
}
