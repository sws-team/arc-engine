#ifndef ARCLOG_H
#define ARCLOG_H

#ifdef SFML_SYSTEM_ANDROID
#include <android/log.h>
#endif
#include <iostream>
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/String.hpp"
#include <ArcVariant>
#include <locale>
#include <codecvt>

#ifdef SFML_SYSTEM_ANDROID
#define log_value(x) __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", std::string(x).c_str());
#endif

#define ARC_ARG_START  inline ArcLog &operator<<(const
#define ARC_ARG_END & value) { \
		*stream() << value << DELIMER;\
		return *this;\
	}

#define FUNC_LOG ArcLog() << __FUNCTION__

class ArcLog
{
public:
	ArcLog() = default;
	ArcLog(bool verbose) :
		verbose(verbose) {

	}
	virtual ~ArcLog() {
		if (end)
			*stream() << std::endl;
	}
	virtual std::ostream* stream() {
		return &std::cout;
	}
	ARC_ARG_START std::string ARC_ARG_END
	ARC_ARG_START char ARC_ARG_END
	ARC_ARG_START int ARC_ARG_END
	ARC_ARG_START unsigned ARC_ARG_END
	ARC_ARG_START uintmax_t ARC_ARG_END
	ARC_ARG_START long ARC_ARG_END
	ARC_ARG_START float ARC_ARG_END
	ARC_ARG_START double ARC_ARG_END

	inline ArcLog &operator<<(const bool& value) {
		*stream() << (value ? "true" : "false") << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(const sf::Vector2f& value) {
		*stream() << value.x << DELIMER << value.y << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(const sf::Vector2i& value) {
		*stream() << value.x << DELIMER << value.y << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(const sf::Vector2u& value) {
		*stream() << value.x << DELIMER << value.y << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(const sf::FloatRect& value) {
		*stream() << value.left << DELIMER << value.top << DELIMER
					 << value.width << DELIMER << value.height << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(char *value) {
		*stream() << std::string(value) << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(const sf::String& str) {
		*stream() << str.toAnsiString() << DELIMER;
		return *this;
	}
	inline ArcLog &operator<<(const ArcVariant& value) {
		std::function<std::string(const ArcVariant& value)> printableValue = nullptr;
		printableValue = [&printableValue](const ArcVariant& value) -> std::string {
			std::string result;
			switch (value.type())
			{
			case ArcVariant::VariantType::INT:
				result = std::to_string(value.toInt());
				break;
			case ArcVariant::VariantType::UINT:
				result = std::to_string(value.toUInt());
				break;
			case ArcVariant::VariantType::FLOAT:
				result = std::to_string(value.toFloat());
				break;
			case ArcVariant::VariantType::DOUBLE:
				result = std::to_string(value.toDouble());
				break;
			case ArcVariant::VariantType::STRING:
				result = value.toString();
				break;
			case ArcVariant::VariantType::BOOLEAN:
				result = value.toBool() ? "true" : "false";
				break;
			case ArcVariant::VariantType::LONGLONG:
				result = std::to_string(value.toLongLong());
				break;
			case ArcVariant::VariantType::LIST: {
				result += '[';
				const std::vector<ArcVariant> list = value.toList();
				for (unsigned i = 0; i < list.size(); ++i) {
					result += printableValue(list[i]);
					if (i != list.size() - 1)
						result += ", ";
				}
				result += ']';
			}
				break;
			case ArcVariant::VariantType::MAP: {
				const std::map<std::string, ArcVariant> map = value.toMap();
				auto last = map.end();
				last--;
				for (auto it = map.begin(); it != map.end(); ++it) {
					result += "'" + it->first + "' = " + printableValue(it->second);
					if (it != last)
						result += ", ";
				}
			}
				break;
			default:
				break;
			}
			return result;
		};
		if (verbose) {
			*stream() << "(" + ArcVariant::typeToName(value.type()) + ")" << DELIMER << printableValue(value) << DELIMER;
		}
		else {
			*stream() << printableValue(value) << DELIMER;
		}
		return *this;
	}
	template<typename T>
	inline ArcLog &operator<<(const std::vector<T>& value) {
		*this << "[";
		for (unsigned i = 0; i < value.size(); ++i) {
			*this << value.at(i);
			if (i != value.size() - 1)
				*stream() << ", ";
		}
		*this << "]";
		return *this;
	}
	template<typename T>
	inline ArcLog &operator<<(const std::map<std::string, T>& value) {
		*this << "[";
		auto last = value.end();
		last--;
		for (auto it = value.begin(); it != value.end(); ++it) {
			*this << "'" << it->first << "' = " << it->second;
			if (it != last)
				*this << ", ";
		}
		*this  << "]";
		return *this;
	}

protected:
	bool end = true;
private:
	static constexpr char DELIMER = ' ';
	bool verbose = false;
};

class ArcErr : public ArcLog
{
public:
	std::ostream* stream() override {
		return &std::cerr;
	}
};

#include <sstream>

class StringLog : public ArcLog
{
public:
	StringLog(bool verbose = false) :
		ArcLog(verbose) {
		end = false;
	};
	std::ostream* stream() override {
		return &ss;
	}
	std::string toString() {
		return ss.str();
	}
private:
	std::ostringstream ss;
};

#undef ARC_ARG_START
#undef ARC_ARG_END

#endif // ARCLOG_H
