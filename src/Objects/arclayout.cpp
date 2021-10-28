#include "arclayout.h"

ArcLayout::ArcLayout(const std::string& name)
	: ArcObject(name)
{
	setType(ArcEngine::LAYOUT);
}

void ArcLayout::init()
{
	ArcObject::init();
	refreshChilds();
}

void ArcLayout::setRows(unsigned count)
{
	if (m_rows == count)
		return;
	m_rows = count;
	if(m_autoSize)
		refreshChilds();
}

void ArcLayout::setColumns(unsigned count)
{
	if (m_columns == count)
		return;
	m_columns = count;
	if(m_autoSize)
		refreshChilds();
}

void ArcLayout::setGrid(unsigned rows, unsigned columns)
{
	if (m_rows == rows && m_columns == columns)
		return;
	this->m_rows = rows;
	this->m_columns = columns;
	if(m_autoSize)
		refreshChilds();
}

void ArcLayout::setAutoSize(bool autoSize)
{
	if (m_autoSize == autoSize)
		return;
	m_autoSize = autoSize;
	refreshChilds();
}

void ArcLayout::setOffsetX(float x)
{
	setOffset(x, m_offset.y);
}

void ArcLayout::setOffsetY(float y)
{
	setOffset(m_offset.x, y);
}

void ArcLayout::setOffset(float x, float y)
{
	setOffset(sf::Vector2f(x, y));
}

void ArcLayout::setOffset(const sf::Vector2f &offset)
{
	if (m_offset == offset)
		return;
	m_offset = offset;
	refreshChilds();
}

float ArcLayout::offsetX() const
{
	return m_offset.x;
}

float ArcLayout::offsetY() const
{
	return m_offset.y;
}

bool ArcLayout::autoSize() const
{
	return m_autoSize;
}

void ArcLayout::updateSize()
{
	ArcObject::updateSize();
	if(!m_autoSize)
		refreshChilds();
}

void ArcLayout::updateScale()
{
	ArcObject::updateScale();
	if(m_autoSize)
		refreshChilds();
}

unsigned ArcLayout::rows() const
{
	return m_rows;
}

unsigned ArcLayout::columns() const
{
	return m_columns;
}

void ArcLayout::refreshChilds()
{
	sf::Vector2f pos = sf::Vector2f(0, 0);
	if (m_autoSize) {
		bool end = false;
		unsigned n = 0;
		float childMaxWidth = 0;
		for (unsigned row = 0; row < m_rows; ++row) {
			float childMaxHeight = 0;
			for (unsigned column = 0; column < m_columns; ++column) {
				end = n == childs.size();
				if (end)
					break;
				ArcObject *child = childs.at(n);
				child->setPos(pos + sf::Vector2f(child->scaledSize().x * child->originX(),
												 child->scaledSize().y * child->originY()));
				pos.x += child->scaledSize().x;
				pos.x += m_offset.x * scaleX();
				if (child->scaledSize().y > childMaxHeight)
					childMaxHeight = child->scaledSize().y;
				n++;
			}
			if (end)
				break;
			if (pos.x > childMaxWidth) {
				childMaxWidth = pos.x;
			}
			pos.y += childMaxHeight;
			pos.x = m_offset.x * scaleX();
			if (row != m_rows - 1)
				pos.y += m_offset.y * scaleY();
		}
		setSize(childMaxWidth, pos.y);
	}
	else {
		const sf::Vector2f sSize = scaledSize();
		float maxHeight = 0;
		for(ArcObject* child : childs) {
			child->setPos(pos);
			pos.x += child->scaledSize().x;
			pos.x += m_offset.x * scaleX();
			if (child->scaledSize().y > maxHeight)
				maxHeight = child->scaledSize().y;
			if (pos.x > sSize.x) {
				pos.x = m_offset.x * scaleX();
				pos.y += maxHeight;
				pos.y += m_offset.y * scaleY();
				maxHeight = 0;
			}
		}
	}
}
