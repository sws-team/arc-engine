#ifndef STDHEADER_H
#define STDHEADER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <functional>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <limits.h>

#if defined(SFML_SYSTEM_ANDROID) || defined(SFML_SYSTEM_IOS)
#define MOBILE_OS
#endif

#ifdef SFML_SYSTEM_ANDROID
#include <android/log.h>
#define watch(x) __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", std::string(x).c_str());
#else
#define watch(x) std::cout << (#x) << " is " << (x) << std::endl
#endif

#endif // STDHEADER_H
