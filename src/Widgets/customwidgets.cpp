#include "customwidgets.h"
#include "arcsprite.h"
#include "engine.h"
#include "managers.h"

CustomWidgets::CustomWidgets()
{

}

CircleScale::CircleScale(const std::string &name, TextureType texture, unsigned sectors)
	: ArcObject(name)
	,m_sectors(sectors)
{
	const sf::Vector2u size = Engine::Instance().texturesManager()->getTexture(texture).getSize();
	for (unsigned i = 0; i < sectors; ++i) {
		ArcSprite *sectorSprite = new ArcSprite("sector_" + std::to_string(i));
		sectorSprite->setTexture(texture);
		sectorSprite->setSize(size.x, size.y);
		sectorSprite->setOrigin(0.5f, 1.f);
		addChild(sectorSprite);
	}
	setCenteredOrigin();
	setSize(size.y * 2, size.y * 2);

	setValue(m_value);
}

void CircleScale::init()
{
	ArcObject::init();
	for (unsigned i = 0; i < m_sectors; ++i) {
		ArcSprite *sectorSprite = static_cast<ArcSprite*>(m_childs.at(i));
		sectorSprite->setRotation(360.f * static_cast<float>(i)/static_cast<float>(m_sectors));
	}
}

unsigned CircleScale::value() const
{
	return m_value;
}

void CircleScale::setValue(unsigned value)
{
	m_value = value;

	for (unsigned i = 0; i < m_sectors; ++i) {
		ArcSprite *sectorSprite = static_cast<ArcSprite*>(m_childs.at(i));
		sectorSprite->setEnabled(i < m_value);
	}
}
