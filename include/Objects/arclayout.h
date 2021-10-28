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

	float offsetX() const;
	float offsetY() const;
	bool autoSize() const;
	unsigned rows() const;
	unsigned columns() const;

	void updateSize() override;
	void updateScale() override;

private:
	unsigned m_rows = 1;
	unsigned m_columns = 1;
	bool m_autoSize = true;
	sf::Vector2f m_offset;

	void refreshChilds();
};

#endif // ARCLAYOUT_H
