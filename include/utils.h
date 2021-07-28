#ifndef UTILS_H
#define UTILS_H

#include "stdheader.h"

namespace Utils {

//https://github.com/SFML/SFML/wiki/Source%3A-cubic-bezier-curve
std::vector<sf::Vector2f> calcCubicBezier(const sf::Vector2f &start,
										  const sf::Vector2f &end,
										  const sf::Vector2f &startControl,
										  const sf::Vector2f &endControl,
										  const size_t numSegments);
}

#endif // UTILS_H
