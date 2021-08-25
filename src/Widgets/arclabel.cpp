#include "arclabel.h"
#include "engine.h"
#include "managers.h"

ArcLabel::ArcLabel(const std::string& name)
	: ArcObject(name, ArcEngine::LABEL)
{
	watch(scaleFactor.x);
	text.setScale(SCALE_FACTOR);
	text.setFont(Engine::Instance().fontManager()->font());
	text.setOutlineThickness(m_borderSize);
	text.setOutlineColor(m_borderColor);
	text.setFillColor(m_color);
}

void ArcLabel::draw(sf::RenderTarget * const target)
{
	ArcObject::draw(target);
	target->draw(text);
}

void ArcLabel::setPos(float x, float y)
{
	ArcObject::setPos(x, y);
	text.setPosition(x, y);
}

void ArcLabel::setOrigin(float x, float y)
{
	ArcObject::setOrigin(x, y);
	text.setOrigin(x, y);
}

void ArcLabel::setScale(float x, float y)
{
	ArcObject::setScale(x, y);
	text.setScale(x, y);
}
