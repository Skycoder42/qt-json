#pragma once

#include <optional>
#include <iserializable.h>

class TestSerializable : public QtJson::ISerializable
{
public:
	double value = 0;
	std::optional<QCborKnownTags> tag = std::nullopt;

	TestSerializable();
	TestSerializable(double value, std::optional<QCborKnownTags> tag = std::nullopt);

	bool operator==(const TestSerializable &other) const;
	bool operator!=(const TestSerializable &other) const;

	QJsonValue toJson(const QtJson::Configuration &) const override;
	void assignJson(const QJsonValue &value, const QtJson::Configuration &) override;
	QCborValue toCbor(const QtJson::Configuration &) const override;
	void assignCbor(const QCborValue &value, const QtJson::Configuration &) override;
};

QDebug operator<<(QDebug debug, const TestSerializable &value);

Q_DECLARE_METATYPE(TestSerializable)
