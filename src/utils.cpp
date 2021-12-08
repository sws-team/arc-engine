#include "utils.h"
#include "arcobject.h"
#include "arcproperties.h"

#include <sstream>

namespace Utils {

std::vector<sf::Vector2f> calcCubicBezier(const sf::Vector2f &start,
										  const sf::Vector2f &end,
										  const sf::Vector2f &startControl,
										  const sf::Vector2f &endControl,
										  const size_t numSegments)
{
	std::vector<sf::Vector2f> ret;
	if (!numSegments) // Any points at all?
		return ret;

	ret.push_back(start); // First point is fixed
	float p = 1.f / numSegments;
	float q = p;
	for (size_t i = 1; i < numSegments; i++, p += q) // Generate all between
		ret.push_back(p * p * p * (end + 3.f * (startControl - endControl) - start) +
					  3.f * p * p * (start - 2.f * startControl + endControl) +
					  3.f * p * (startControl - start) + start);
	ret.push_back(end); // Last point is fixed
	return ret;
}

std::string to_string_with_precision(const float a_value, const int n)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::string(out.str());
}
#ifdef ARC_DEBUG
sf::Color convertToColor(const ImVec4 &color)
{
	sf::Color result;
	result.r = ColorProperty::ratioToColor(color.x);
	result.g = ColorProperty::ratioToColor(color.y);
	result.b = ColorProperty::ratioToColor(color.z);
	result.a = ColorProperty::ratioToColor(color.w);
	return result;
}

ImVec4 convertFromColor(const sf::Color &color)
{
	ImVec4 result;
	result.x = ColorProperty::colorToRatio(color.r);
	result.y = ColorProperty::colorToRatio(color.g);
	result.z = ColorProperty::colorToRatio(color.b);
	result.w = ColorProperty::colorToRatio(color.a);
	return result;
}
#endif

sf::Vector2f getPosition(ArcObject *from, ArcObject *to)
{
	auto findParent = [](ArcObject *from, ArcObject *to) -> ArcObject* {
		ArcObject *currentObjectFrom = from;
		while(currentObjectFrom != nullptr) {
			ArcObject *currentObjectTo = to;
			while(currentObjectTo != nullptr) {
				if (currentObjectTo == currentObjectFrom) {
					return currentObjectTo;
				}
				currentObjectTo = currentObjectTo->parent();
			}
			currentObjectFrom = currentObjectFrom->parent();
		}
		return nullptr;
	};
	ArcObject *parent = findParent(from, to);
	if (parent == nullptr)
		return sf::Vector2f();

	auto getGlobalPos = [](ArcObject *object, ArcObject *parent) -> sf::Vector2f {
		sf::Vector2f pos;
		ArcObject *currentObject = object;
		while(currentObject != nullptr) {
			pos += currentObject->pos();
			if (currentObject == parent)
				break;
			currentObject = currentObject->parent();
		}
		return pos;
	};
	const sf::Vector2f posFrom = getGlobalPos(from, parent);
	const sf::Vector2f posTo = getGlobalPos(to, parent);
	return posTo - posFrom;
}

}
