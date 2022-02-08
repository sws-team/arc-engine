#include "customwidgets.h"
#include "arcsprite.h"
#include "engine.h"
#include "managers.h"
#ifdef ARC_DEBUG
#include "imgui.h"
#include "imgui-SFML.h"
#include "imconfig-SFML.h"
#endif

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

void CircleScale::initTransform()
{
	ArcObject::initTransform();
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

PathObject::PathObject(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::PATH);
}

std::vector<sf::Vector2f> PathObject::path() const
{
	return m_path;
}

void PathObject::draw(sf::RenderTarget * const target)
{
#ifdef ARC_DEBUG
	if (!m_debug) {
		ArcObject::draw(target);
		return;
	}

	auto addPoint = [this](const sf::Vector2f& point, bool addCircle,
			std::vector<sf::CircleShape> *points,
			std::vector<sf::Vertex> *vertices) {
		const sf::Vector2f scaledPos = sf::Vector2f(point.x * scaleFactor.x,
													point.y * scaleFactor.y);
		if (addCircle) {
			sf::CircleShape circle;
			circle.setPosition(scaledPos - sf::Vector2f(radius, radius));
			circle.setFillColor(m_pointColor);
			circle.setRadius(radius);
			points->push_back(circle);
		}
		const sf::Vertex v = sf::Vertex(scaledPos, m_color);
		vertices->push_back(v);
	};
	std::vector<sf::CircleShape> points;
	std::vector<sf::Vertex> vertices;
	for (int i = 0; i < static_cast<int>(m_path.size()) - 1; ++i) {
		addPoint(m_path[i], true, &points, &vertices);
		addPoint(m_path[i + 1], i == static_cast<int>(m_path.size()) - 2, &points, &vertices);
	}
	if (!vertices.empty())
		target->draw(&vertices[0], vertices.size(), sf::Lines, m_transform);
	for(const sf::CircleShape& circle : points)
		target->draw(circle, m_transform);
#endif
	ArcObject::draw(target);
}

#ifdef ARC_DEBUG
void PathObject::debug()
{
	unsigned i = 0;
	for (auto it = m_path.begin(); it != m_path.end(); ++i) {
		sf::Vector2f pos = *it;
		const std::string idStr = std::to_string(i);
		{//x
			float x = pos.x;
			ImGui::DragFloat(std::string("##X" + idStr).c_str(), &x, 1.f, 0, 9999);
			ImGui::SameLine();
			ImGui::TextUnformatted("X");
			it->x = x;
		}
		//ImGui::SameLine();
		{//x
			float y = pos.y;
			ImGui::DragFloat(std::string("##Y" + idStr).c_str(), &y, 1.f, 0, 9999);
			ImGui::SameLine();
			ImGui::TextUnformatted("Y");
			it->y = y;
		}
		//ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			it = m_path.erase(it);
		}
		else
			++it;
	}
	if (ImGui::Button("Add")) {
		m_path.push_back(sf::Vector2f(0, 0));
	}
}
#endif

void PathObject::setPath(const std::vector<sf::Vector2f> &path)
{
	m_path = path;
}
