#ifndef ARCLOG_H
#define ARCLOG_H

#ifdef SFML_SYSTEM_ANDROID
#include <android/log.h>
#endif
#include <iostream>

#ifdef ARC_DEBUG

#ifdef SFML_SYSTEM_ANDROID
#define log_value(x) __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", std::string(x).c_str());
#endif

#define ARC_ARG_START  inline ArcLog &operator<<(const
#define ARC_ARG_END & value) { \
		std::cout << value << DELIMER;\
		return *this;\
	}

class ArcLog
{
public:
	ArcLog() = default;
	~ArcLog() {
		std::cout << std::endl;
	}
	ARC_ARG_START std::string ARC_ARG_END
	ARC_ARG_START char ARC_ARG_END
	ARC_ARG_START bool ARC_ARG_END
	ARC_ARG_START int ARC_ARG_END
	ARC_ARG_START unsigned ARC_ARG_END
	ARC_ARG_START long ARC_ARG_END
	ARC_ARG_START float ARC_ARG_END
	ARC_ARG_START double ARC_ARG_END

private:
	static constexpr char DELIMER = ' ';
};

#undef ARC_ARG_START
#undef ARC_ARG_END

#endif // ARC_DEBUG

#endif // ARCLOG_H
