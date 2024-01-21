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
		LIST,
		MAP
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
	ArcVariant(const std::vector<ArcVariant>& v);
	ArcVariant(const std::map<std::string, ArcVariant>& v);

	VariantType type() const;
	bool isValid() const;

	int toInt() const;
	unsigned toUInt() const;
	float toFloat() const;
	double toDouble() const;
	std::string toString() const;
	bool toBool() const;
	uintmax_t toLongLong() const;
	std::vector<ArcVariant> toList() const;
	std::map<std::string, ArcVariant> toMap() const;

	template<class T> T data() const {
		return std::any_cast<T>(value);
	}
	std::any data() const;

	bool operator == (const ArcVariant& other) const;

	static std::string typeToName(VariantType type);

private:
	std::any value;
	VariantType m_type;

	const static std::map<VariantType, std::string> TYPE_NAMES;
};
#endif // ARCVARIANT_H
