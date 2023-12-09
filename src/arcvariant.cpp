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
	{ STRING, "String" },
	{ BOOLEAN, "Boolean" },
	{ LONGLONG, "LongLong" },
	{ LIST, "List" },
	{ MAP, "Map" }
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

ArcVariant::ArcVariant(bool v)
{
	INIT_VARIANT(v);
	m_type = BOOLEAN;
}

ArcVariant::ArcVariant(uintmax_t v)
{
	INIT_VARIANT(v);
	m_type = LONGLONG;
}

ArcVariant::ArcVariant(const char *v)
	: ArcVariant(std::string(v))
{

}

ArcVariant::ArcVariant(const std::vector<ArcVariant> &v)
{
	INIT_VARIANT(v);
	m_type = LIST;
}

ArcVariant::ArcVariant(const std::map<std::string, ArcVariant> &v)
{
	INIT_VARIANT(v);
	m_type = MAP;
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

bool ArcVariant::toBool() const
{
	CAST_VALUE(VariantType::BOOLEAN, bool)
}

uintmax_t ArcVariant::toLongLong() const
{
	CAST_VALUE(VariantType::LONGLONG, uintmax_t);
}

std::vector<ArcVariant> ArcVariant::toList() const
{
	CAST_VALUE(VariantType::LIST, std::vector<ArcVariant>);
}

std::map<std::string, ArcVariant> ArcVariant::toMap() const
{
	using MapType = std::map<std::string, ArcVariant>;
	CAST_VALUE(VariantType::MAP, MapType);
}

std::any ArcVariant::data() const
{
	return value;
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
	case BOOLEAN:
		return this->toBool() == other.toBool();
	case LONGLONG:
		return this->toLongLong() == other.toLongLong();
	case LIST:
		return this->toList() == other.toList();
	case MAP:
		return this->toMap() == other.toMap();
	default:
		break;
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

	const std::string valueStr = printableValue();
	return "(" + typeName + ") " + valueStr;
}

std::string ArcVariant::printableValue() const
{
	std::string result;
	switch (m_type)
	{
	case ArcVariant::VariantType::INT:
		result = std::to_string(toInt());
		break;
	case ArcVariant::VariantType::UINT:
		result = std::to_string(toUInt());
		break;
	case ArcVariant::VariantType::FLOAT:
		result = std::to_string(toFloat());
		break;
	case ArcVariant::VariantType::DOUBLE:
		result = std::to_string(toDouble());
		break;
	case ArcVariant::VariantType::STRING:
		result = toString();
		break;
	case ArcVariant::VariantType::BOOLEAN:
		result = toBool() ? "true" : "false";
		break;
	case ArcVariant::VariantType::LONGLONG:
		result = std::to_string(toLongLong());
		break;
	case ArcVariant::VariantType::LIST: {
		const std::vector<ArcVariant> list = toList();
		for (unsigned i = 0; i < list.size(); ++i) {
			result += list[i].printableValue();
			if (i != list.size() - 1)
				result += ", ";
		}
	}
		break;
	case ArcVariant::VariantType::MAP: {
		const std::map<std::string, ArcVariant> map = toMap();
		auto last = map.end();
		last--;
		for (auto it = map.begin(); it != map.end(); ++it) {
			result += "'" + it->first + "' = " + it->second.printableValue();
			if (it != last)
				result += ", ";
		}
	}
		break;
	default:
		break;
	}
	return result;
}

