#ifndef ARCENGINEUTILS_H
#define ARCENGINEUTILS_H

#include "enginedef.h"
#ifdef ARC_DEBUG
#include "imgui.h"
#endif

#define PIXEL_TO_POS(x, y) Engine::Instance().window()->mapPixelToCoords(sf::Vector2i(x, y), *Engine::Instance().window()->view());

namespace ArcEngine {
	ArcObject *findChild(const std::string& name);
	ArcObject *findChildPath(const std::string& path);

	//https://github.com/SFML/SFML/wiki/Source%3A-cubic-bezier-curve
	std::vector<sf::Vector2f> calcCubicBezier(const sf::Vector2f &start,
											  const sf::Vector2f &end,
											  const sf::Vector2f &startControl,
											  const sf::Vector2f &endControl,
											  const size_t numSegments);

	std::string to_string_with_precision(const float a_value, const int n = 6);

#ifdef ARC_DEBUG
	sf::Color convertToColor(const ImVec4& color);
	ImVec4 convertFromColor(const sf::Color& color);
#endif
	sf::Vector2f getPosition(ArcObject* from, ArcObject* to);
	float getAngle(ArcObject* from, ArcObject* to);
	float getAngle(const sf::Vector2f& from, const sf::Vector2f& to);
	sf::Color randomColor(int seed = 0);

	std::vector<std::string> split(const std::string& str, const std::string& delimiter);
}

#endif // ARCENGINEUTILS_H
