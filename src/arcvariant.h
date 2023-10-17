#ifndef ARCVARIANT_H
#define ARCVARIANT_H

#include "stdheader.h"

class ArcVariant
{
public:
	enum VariantType
	{
		INVALID,
		INT,
		UINT,
		FLOAT,
		DOUBLE,
		STRING,
		BOOLEAN,
		LONGLONG,
	};

	ArcVariant();
	ArcVariant(int v);
	ArcVariant(unsigned v);
	ArcVariant(float v);
	ArcVariant(double v);
	ArcVariant(const char* v);
	ArcVariant(const std::string& v);
	ArcVariant(bool v);
	ArcVariant(uintmax_t v);

	VariantType type() const;
	bool isValid() const;

	int toInt() const;
	unsigned toUInt() const;
	float toFloat() const;
	double toDouble() const;
	std::string toString() const;
	bool toBool() const;
	uintmax_t toLongLong() const;

	template<class T> T data() const {
		return std::any_cast<T>(value);
	}

	bool operator == (const ArcVariant& other) const;

	std::string printable() const;
	std::string printableValue() const;

private:
	std::any value;
	VariantType m_type;

	const static std::map<VariantType, std::string> TYPE_NAMES;
};
#endif // ARCVARIANT_H
