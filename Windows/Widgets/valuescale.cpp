#include "valuescale.h"

ValueScale::ValueScale():
  m_scales(10)
  ,currentTriangle(0)
  ,m_max(100)
  ,m_min(0)
{
	update();
}

void ValueScale::draw(RenderTarget *target)
{
	for(const ConvexShape& shape: triangles)
		target->draw(shape);
}

void ValueScale::event(Event *event)
{
	if (event->type == Event::MouseButtonReleased)
	{
		const Vector2f localPos = Vector2f(event->mouseButton.x - m_pos.x, event->mouseButton.y - m_pos.y);
		if (localPos.y < m_size.y || localPos.y > 0)
			return;

		for (int i = triangles.size() - 1; i >= 0 ; --i)
		{
			const int x = m_size.x - m_size.x / m_scales * i;
			if (localPos.x > 0 && localPos.x < x)
			{
				currentTriangle = i;
				break;
			}
		}
		updateValue();
	}
}

void ValueScale::update()
{
	updateTriangle();
}

void ValueScale::setScalesCount(int scales)
{
	m_scales = scales;
	updateTriangle();
}

void ValueScale::setRange(float min, float max)
{
	m_min = min;
	m_max = max;
}

void ValueScale::setMinValue(float min)
{
	m_min = min;
}

void ValueScale::setMaxValue(float max)
{
	m_max = max;
}

float ValueScale::value() const
{
	return currentTriangle * m_max/m_scales - m_min;
}

void ValueScale::setValue(float value)
{
	const float currentValue = m_max - value;
	currentTriangle = currentValue * m_scales/m_max + m_min;
	updateTriangle();
}

void ValueScale::updateTriangle()
{
	triangles.clear();
	for (int i = 0; i < m_scales; ++i)
	{
		ConvexShape shape;
		shape.setPointCount(3);
		const int x = static_cast<int>(trunc(m_size.x - m_size.x / m_scales * i));
		const int y = static_cast<int>(trunc(m_size.y - m_size.y / m_scales * i));

		shape.setPoint(0, Vector2f(0, 0));
		shape.setPoint(1, Vector2f(x, y));
		shape.setPoint(2, Vector2f(x, 0));

		shape.setPosition(m_pos);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color::Black);

		triangles.push_back(shape);
	}
	updateValue();
}

void ValueScale::updateValue()
{
	for (unsigned int i = 0; i < triangles.size(); ++i)
	{
		if (i < currentTriangle)
		{
			triangles[i].setFillColor(Color::Red);
		}
		else
			triangles[i].setFillColor(Color::Blue);
	}
}
