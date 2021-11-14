#ifndef ARCLAYOUT_H
#define ARCLAYOUT_H

#include "arcobject.h"

class ArcLayout : public ArcObject
{
public:
	ArcLayout(const std::string& name);

	void init() override;

	void setRows(unsigned count);
	void setColumns(unsigned count);
	void setGrid(unsigned rows, unsigned columns);
	void setAutoSize(bool autoSize);

	void setOffsetX(float x);
	void setOffsetY(float y);
	void setOffset(float x, float y);
	void setOffset(const sf::Vector2f& offset);
	void setBorderOffset(const sf::Vector2f &borderOffset);
	void setBorderOffset(float x, float y);

	float offsetX() const;
	float offsetY() const;
	bool autoSize() const;
	unsigned rows() const;
	unsigned columns() const;
	sf::Vector2f borderOffset() const;

	void updateSize() override;
	void updateScale() override;

	bool skipDisabledElements() const;
	void setSkipDisabledElements(bool skipDisabledElements);

	void updateLayout();
	void clear();

private:
	unsigned m_rows = 1;
	unsigned m_columns = 1;
	bool m_autoSize = true;
	bool m_skipDisabledElements = false;
	sf::Vector2f m_offset;
	sf::Vector2f m_borderOffset;

	void refreshChilds();
};

#endif // ARCLAYOUT_H
