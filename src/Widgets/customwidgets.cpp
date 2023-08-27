#include "customwidgets.h"
#include <ArcSprite>
#include "engine.h"
#include "managers.h"
#include "mainwindow.h"
#ifdef ARC_DEBUG
#include "imgui.h"
#include "imconfig-SFML.h"
#endif

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
		sf::Vector2f scaledPos = sf::Vector2f(point.x, point.y);
		if (enabledScaleFactor) {
			scaledPos = sf::Vector2f(scaledPos.x * scaleFactor.x,
									 scaledPos.y * scaleFactor.y);
		}
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
		m_path.push_back(m_path.empty()?sf::Vector2f(0, 0):m_path.back());
	}
}
#endif

void PathObject::setPath(const std::vector<sf::Vector2f> &path)
{
	m_path = path;
}

ZoomView::ZoomView(const std::string &name)
	: ArcObject(name)
{
	setType(ArcEngine::OBJECT_TYPE::ZOOM_VIEW);
	view.setSize(RESOLUTION);
}

void ZoomView::setView(const sf::Vector2f &pos)
{
	view.setCenter(pos);
	fitScreen();
}

void ZoomView::setView(const float x, const float y)
{
	setView(sf::Vector2f(x, y));
}

sf::Vector2f ZoomView::viewPos() const
{
	return view.getCenter();
}

void ZoomView::zoomIn()
{
	if (zoomRatio > MAX_ZOOM)
		return;
	view.zoom(ZOOM_STEP);
	zoomFactor *= ZOOM_STEP;
	zoomRatio++;
	fitScreen();
}

void ZoomView::zoomOut()
{
	if (zoomRatio < -MAX_ZOOM)
		return;
	view.zoom(1/ZOOM_STEP);
	zoomFactor /= ZOOM_STEP;
	zoomRatio--;
	fitScreen();
}

void ZoomView::draw(sf::RenderTarget * const target)
{
	Engine::Instance().window()->setView(view);
	ArcObject::draw(target);
	Engine::Instance().window()->setView(*Engine::Instance().window()->view());
}

bool ZoomView::eventFilter(sf::Event *event)
{
	switch (event->type)
	{
	case sf::Event::MouseWheelScrolled:
	{
		if (event->mouseWheelScroll.delta < 0)
			zoomOut();
		else
			zoomIn();
	}
		break;
	case sf::Event::MouseButtonPressed:
	{
		move = true;
		startMovePos = sf::Vector2i(event->mouseButton.x, event->mouseButton.y);
	}
		break;
	case sf::Event::MouseMoved:
	{
		if (move) {
			const sf::Vector2i pixelPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
			if (pixelPos == startMovePos)
				break;

			const sf::Vector2f pos = Engine::Instance().window()->mapPixelToCoords(pixelPos, view);
			const sf::Vector2f movePos = Engine::Instance().window()->mapPixelToCoords(startMovePos, view);
			const sf::Vector2f offsetPos = pos - movePos;
			const sf::Vector2f center = view.getCenter() - offsetPos;

			view.setCenter(center);
			startMovePos = pixelPos;
			fitScreen();
		}
	}
		break;
	case sf::Event::MouseButtonReleased:
	{
		move = false;
	}
		break;
	default:
		break;
	}
	return ArcObject::eventFilter(event);
}

sf::View *ZoomView::getView()
{
	return &view;
}

void ZoomView::fitScreen()
{
	const sf::Vector2f scf = enabledScaleFactor ? scaleFactor : sf::Vector2f(1.f, 1.f);
	sf::Vector2f newCenter = view.getCenter();
	const sf::Vector2f topLeft = Engine::Instance().window()->mapPixelToCoords(sf::Vector2i(0, 0), view);
	const sf::Vector2f bottomRight = Engine::Instance().window()->mapPixelToCoords(Engine::Instance().settingsManager()->getResolution(), view);
	if (topLeft.x < 0.f) {
		newCenter.x = view.getSize().x/2;
	}
	if (bottomRight.x > size().x * scf.x) {
		newCenter.x = size().x * scf.x - view.getSize().x/2;
	}
	if (topLeft.y < 0.f) {
		newCenter.y = view.getSize().y/2;
	}
	if (bottomRight.y > size().y * scf.y) {
		newCenter.y = size().y * scf.y - view.getSize().y/2;
	}
	if (newCenter != view.getCenter()) {
		view.setCenter(newCenter);
	}
}
