#ifndef UTILS_H
#define UTILS_H

#include "stdheader.h"
#ifdef ARC_DEBUG
#include "imgui.h"
#endif
namespace Utils {

//https://github.com/SFML/SFML/wiki/Source%3A-cubic-bezier-curve
std::vector<sf::Vector2f> calcCubicBezier(const sf::Vector2f &start,
										  const sf::Vector2f &end,
										  const sf::Vector2f &startControl,
										  const sf::Vector2f &endControl,
										  const size_t numSegments);

std::string to_string_with_precision(const float a_value, const int n = 6);

#ifdef ARC_DEBUG
constexpr float COLOR_CONST = 255.f;
sf::Color convertToColor(const ImVec4& color);
ImVec4 convertFromColor(const sf::Color& color);
#endif
}

#endif // UTILS_H