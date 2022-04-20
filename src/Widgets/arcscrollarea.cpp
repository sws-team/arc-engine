#include <arcscrollarea.h>
#include <mainwindow.h>
#include <engine.h>
#include <arcrect.h>

ArcScrollArea::ArcScrollArea(const std::string &name)
	: ArcObject(name)
{
	horizontalScroll = new ArcRect("horizontalScroll");
	horizontalScroll->setSize(1, scrollBarSize);
	horizontalScroll->setColor(ArcEngine::GrayColor);
	addChild(horizontalScroll);

	verticalScroll = new ArcRect("verticalScroll");
	verticalScroll->setSize(scrollBarSize, 1);
	verticalScroll->setColor(ArcEngine::GrayColor);
	addChild(verticalScroll);
}

void ArcScrollArea::draw(sf::RenderTarget * const target)
{
	target->draw(rect, m_transform);
	if (verticalScrollEnabled)
		verticalScroll->draw(target);
	if (horizontalScrollEnabled)
		horizontalScroll->draw(target);
}

bool ArcScrollArea::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::MouseWheelScrolled) {
		if (verticalScrollEnabled && event->mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
			if (event->mouseWheelScroll.delta > 0) {
				scrollY -= scrollStepY;
				if (scrollY < 0)
					scrollY = 0;
			}
			else {
				scrollY += scrollStepY;
				if (scrollY > maxY)
					scrollY = maxY;
			}
			updateArea();
		}
		else if (horizontalScrollEnabled && event->mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
			if (event->mouseWheelScroll.delta > 0) {
				scrollX -= scrollStepX;
				if (scrollX < 0)
					scrollX = 0;
			}
			else {
				scrollX += scrollStepX;
				if (scrollX > maxX)
					scrollX = maxX;
			}
			updateArea();
		}
		return true;
	}
	return false;
}

void ArcScrollArea::updatePos()
{
	rect.setPosition(scaledGlobalPos());
	ArcObject::updatePos();
}

void ArcScrollArea::updateScale()
{
	rect.setScale(scaledGlobalScale());
	ArcObject::updateScale();
}

void ArcScrollArea::updateOrigin()
{
	rect.setOrigin(globalOrigin());
	ArcObject::updateOrigin();
}

void ArcScrollArea::updateSize()
{
	rect.setSize(size());
	ArcObject::updateSize();
	initArea();
}

void ArcScrollArea::update()
{
	if (m_repaint)
		initArea();
	ArcObject::update();
}

void ArcScrollArea::setWidget(ArcObject *object)
{
	m_object = object;
	m_object->setScaleFactorEnabled(false);
	addChild(m_object);
	initArea();
}

void ArcScrollArea::setNeedRepaint(bool repaint)
{
	m_repaint = repaint;
}

void ArcScrollArea::initArea()
{
	if (m_object == nullptr)
		return;
	if (texture != nullptr) {
		delete texture;
	}
	horizontalScrollEnabled = m_object->width() > this->width();
	verticalScrollEnabled = m_object->height() > this->height();
	areaSize = sf::Vector2u(static_cast<unsigned>(width()), static_cast<unsigned>(height()));
	if (horizontalScrollEnabled)
		areaSize.x -= scrollBarSize;
	if (verticalScrollEnabled)
		areaSize.y -= scrollBarSize;

	texture = new sf::RenderTexture();
	if (!texture->create(static_cast<unsigned>(m_object->width()), static_cast<unsigned>(m_object->height())))
		return;

	texture->clear(sf::Color::Transparent);

	m_object->draw(texture);
	texture->display();

	rect.setTexture(&texture->getTexture(), true);

	maxX = static_cast<int>(m_object->width() - width());
	maxY = static_cast<int>(m_object->height() - height());
	scrollStepX = static_cast<int>(step * maxX);
	scrollStepY = static_cast<int>(step * maxY);

	horizontalScroll->setPos(0, areaSize.y - scrollBarSize);
	horizontalScroll->setEnabled(horizontalScrollEnabled);
	const float kX = static_cast<float>(areaSize.x) / m_object->width();
	const float w = areaSize.y * kX;
	verticalScroll->setHeight(w);

	verticalScroll->setPos(areaSize.x - scrollBarSize, 0);
	verticalScroll->setEnabled(verticalScrollEnabled);
	const float kY = static_cast<float>(areaSize.y) / m_object->height();
	const float h = areaSize.y * kY;
	verticalScroll->setHeight(h);

	updateArea();
}

void ArcScrollArea::updateArea()
{
	if (horizontalScrollEnabled) {
		const float k = static_cast<float>(scrollX) / static_cast<float>(maxX);
		float areaX = areaSize.x - horizontalScroll->width();
		if (!verticalScrollEnabled)
			areaX += scrollBarSize;
		const float x = areaX * k;
		horizontalScroll->setX(x);
	}
	if (verticalScrollEnabled) {
		const float k = static_cast<float>(scrollY) / static_cast<float>(maxY);
		float areaY = areaSize.y - verticalScroll->height();
		if (!horizontalScrollEnabled)
			areaY += scrollBarSize;
		const float y = areaY * k;
		verticalScroll->setY(y);
	}
	rect.setTextureRect(sf::IntRect(scrollX, scrollY, areaSize.x, areaSize.y));
}
