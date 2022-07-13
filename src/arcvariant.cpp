#include "arcvariant.h"

#define INIT_VARIANT(x) value = std::make_any<decltype(v)>(v);
#define CAST_VALUE(x, y) \
	if (m_type != x)\
		return y();\
	if (value.type() != typeid(y))\
		return y();\
	try {\
		const y v = std::any_cast<y>(value);\
		return v;\
	}\
	catch (const std::bad_any_cast& exception) {\
		return y();\
	}\

const std::map<ArcVariant::VariantType, std::string> ArcVariant::TYPE_NAMES = {
	{ INT, "Integer" },
	{ UINT, "Unsigned" },
	{ FLOAT, "Float" },
	{ DOUBLE, "Double" },
	{ STRING, "String" }
};

ArcVariant::ArcVariant()
{
	m_type = INVALID;
}

ArcVariant::ArcVariant(int v)
{
	INIT_VARIANT(v);
	m_type = INT;
}

ArcVariant::ArcVariant(unsigned v)
{
	INIT_VARIANT(v);
	m_type = UINT;
}

ArcVariant::ArcVariant(float v)
{
	INIT_VARIANT(v);
	m_type = FLOAT;
}

ArcVariant::ArcVariant(double v)
{
	INIT_VARIANT(v);
	m_type = DOUBLE;
}

ArcVariant::ArcVariant(const std::string &v)
{
	INIT_VARIANT(v);
	m_type = STRING;
}

ArcVariant::ArcVariant(const char *v)
	: ArcVariant(std::string(v))
{

}

ArcVariant::VariantType ArcVariant::type() const
{
	return m_type;
}

bool ArcVariant::isValid() const
{
	return value.has_value();
}

int ArcVariant::toInt() const
{
	CAST_VALUE(VariantType::INT, int)
}

unsigned ArcVariant::toUInt() const
{
	CAST_VALUE(VariantType::UINT, unsigned)
}

float ArcVariant::toFloat() const
{
	CAST_VALUE(VariantType::FLOAT, float)
}

double ArcVariant::toDouble() const
{
	CAST_VALUE(VariantType::DOUBLE, double)
}

std::string ArcVariant::toString() const
{
	CAST_VALUE(VariantType::STRING, std::string)
}

bool ArcVariant::operator ==(const ArcVariant &other) const {
	if (this->m_type != other.m_type)
		return false;
	switch (m_type)
	{
	case INT:
		return this->toInt() == other.toInt();
	case UINT:
		return this->toUInt() == other.toUInt();
	case FLOAT:
		return this->toFloat() == other.toFloat();
	case DOUBLE:
		return this->toDouble() == other.toDouble();
	case STRING:
		return this->toString() == other.toString();
	}
	return false;
}

std::string ArcVariant::printable() const
{
	std::string typeName;
	if (TYPE_NAMES.find(m_type) != TYPE_NAMES.end())
		 typeName = TYPE_NAMES.at(m_type);
	else
		typeName = std::string("Unknown");

	std::string valueStr;
	switch (m_type)
	{
	case ArcVariant::VariantType::INT:
		valueStr = std::to_string(toInt());
		break;
	case ArcVariant::VariantType::UINT:
		valueStr = std::to_string(toUInt());
		break;
	case ArcVariant::VariantType::FLOAT:
		valueStr = std::to_string(toFloat());
		break;
	case ArcVariant::VariantType::DOUBLE:
		valueStr = std::to_string(toDouble());
		break;
	case ArcVariant::VariantType::STRING:
		valueStr = toString();
		break;
	default:
		break;
	}
	return typeName + " " + valueStr;
}

