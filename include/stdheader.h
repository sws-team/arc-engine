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

#ifdef OS_ANDROID
#include <android/log.h>
#endif

#define  A_LOG(...)  __android_log_print(ANDROID_LOG_INFO, "=============LOG", __VA_ARGS__)
#define watch(x) std::cout << (#x) << " is " << (x) << std::endl

#endif // STDHEADER_H
