#ifndef ARCLOG_H
#define ARCLOG_H

#ifdef SFML_SYSTEM_ANDROID
#include <android/log.h>
#endif
#include <iostream>
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/String.hpp"

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
	~ArcLog() {
		*stream() << std::endl;
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
	inline ArcLog &operator<<(const std::wstring& wstr) {
		*wstream() << wstr << DELIMER;
		return *this;
	}
	virtual std::ostream* stream() {
		return &std::cout;
	}
	virtual std::wostream* wstream() {
		return &std::wcout;
	}

private:
	static constexpr char DELIMER = ' ';
};

class ArcErr : public ArcLog
{
public:
	std::ostream* stream() override {
		return &std::cerr;
	}
	std::wostream* wstream() override {
		return &std::wcerr;
	}
};

#undef ARC_ARG_START
#undef ARC_ARG_END

#endif // ARCLOG_H
