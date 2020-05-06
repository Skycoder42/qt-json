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

	QJsonValue toJson(const QtJson::CommonConfiguration &) const override;
	void assignJson(const QJsonValue &value, const QtJson::CommonConfiguration &) override;
	QCborValue toCbor(const QtJson::CommonConfiguration &) const override;
	void assignCbor(const QCborValue &value, const QtJson::CommonConfiguration &) override;
};

QDebug operator<<(QDebug debug, const TestSerializable &value);
