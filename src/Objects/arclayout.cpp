#include "arclayout.h"

ArcLayout::ArcLayout(const std::string& name)
	: ArcObject(name)
{
	setType(ArcEngine::LAYOUT);
}

void ArcLayout::initTransform()
{
	ArcObject::initTransform();
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

void ArcLayout::updateOrigin()
{
	ArcObject::updateOrigin();
	if(m_autoSize)
		refreshChilds();
}

bool ArcLayout::skipDisabledElements() const
{
	return m_skipDisabledElements;
}

void ArcLayout::setSkipDisabledElements(bool skipDisabledElements)
{
	m_skipDisabledElements = skipDisabledElements;
}

void ArcLayout::updateLayout()
{
	if (isEnabled())
		refreshChilds();
}

void ArcLayout::clear()
{
	for(ArcObject* child : m_childs) {
		delete child;
	}
	m_childs.clear();
}

sf::Vector2f ArcLayout::borderOffset() const
{
	return m_borderOffset;
}

void ArcLayout::setBorderOffset(const sf::Vector2f &borderOffset)
{
	m_borderOffset = borderOffset;
}

void ArcLayout::setBorderOffset(float x, float y)
{
	setBorderOffset(sf::Vector2f(x, y));
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
	sf::Vector2f pos = m_borderOffset;
	pos.x -= originX() * width();
	pos.y -= originY() * height();
	const float baseX = pos.x;
	const float baseY = pos.y;
	if (m_autoSize) {
		bool end = false;
		unsigned n = 0;
		float rowMaxWidth = 0;
		for (unsigned row = 0; row < m_rows; ++row) {
			float childMaxHeight = 0;
			float rowWidth = 0;
			for (unsigned column = 0; column < m_columns; ++column) {
				end = n == m_childs.size();
				if (end)
					break;
				ArcObject *child = m_childs.at(n++);
				if (!child->isEnabled())
					continue;
				const float childWidth = child->size().x;
				child->setPos(pos + sf::Vector2f(childWidth * child->originX(),
												 child->size().y * child->originY()));
				pos.x += childWidth;
//				rowWidth += childWidth;
				rowWidth += child->size().x;
				if (column < m_columns - 1) {
					const float actualOffset = m_offset.x * globalScale(true).x;
					pos.x += actualOffset;
					rowWidth += actualOffset;
				}
				if (child->size().y > childMaxHeight)
					childMaxHeight = child->size().y;
			}
			if (end)
				break;
			if (rowWidth > rowMaxWidth) {
				rowMaxWidth = rowWidth;
			}
			pos.y += childMaxHeight;
			pos.x = baseX;
			if (row != m_rows - 1)
				pos.y += m_offset.y * globalScale(true).y;
		}
		setSize(rowMaxWidth, pos.y - baseY);
	}
	else {
		const float maxWidth = scaledSize().x + (baseX < 0 ? baseX : 0);
		float maxHeight = 0;
		for(ArcObject* child : m_childs) {
			if (!child->isEnabled())
				continue;
			const float childWidth = child->size().x;
			child->setPos(pos + sf::Vector2f(childWidth * child->originX(),
											 child->size().y * child->originY()));
			pos.x += childWidth;
			const float actualOffset = m_offset.x * globalScale(true).x;
			pos.x += actualOffset;

			if (child->size().y > maxHeight)
				maxHeight = child->size().y;

			if (pos.x >= maxWidth - 1) {
				pos.x = baseX;
				pos.y += maxHeight;
				pos.y += m_offset.y * globalScale().y;
				maxHeight = 0;
			}
		}
	}
	for(ArcObject* child : m_childs) {
		if (child->type() != ArcEngine::LAYOUT)
			continue;
		ArcLayout *layout = static_cast<ArcLayout*>(child);
		layout->updateLayout();
	}
}

sf::Vector2f ArcLayout::scaledGlobalPos() const
{
	return scaledGlobalOffset() + ArcObject::scaledGlobalPos();
}

sf::Vector2f ArcLayout::scaledGlobalOffset() const
{
	sf::Vector2f gPos = m_borderOffset;
	const sf::Vector2f gScale = globalScale(true);
	gPos.x *= gScale.x * scaleFactor.x;
	gPos.y *= gScale.y * scaleFactor.y;
	return gPos;
}
