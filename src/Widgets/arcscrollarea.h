#ifndef ARCSCROLLAREA_H
#define ARCSCROLLAREA_H

#include <arcobject.h>

class ArcScrollArea : public ArcObject
{
public:
	ArcScrollArea(const std::string& name);

	void draw(sf::RenderTarget *const target) override;
	bool eventFilter(sf::Event *event) override;

	void setWidget(ArcObject* object);

	void setNeedRepaint(bool repaint);
	bool isNeedRepaint() const;

protected:
	void updatePos() override;
	void updateScale() override;
	void updateOrigin() override;
	void updateSize() override;

	void update() override;
private:
	ArcObject *m_object = nullptr;
	sf::View view;
	float m_scrollBarWidth = 20.f;
	ArcRect *horizontalScroll = nullptr;
	ArcRect *verticalScroll = nullptr;
	sf::RectangleShape rect;
	sf::RenderTexture *texture = nullptr;
	int scrollX = 0;
	int scrollY = 0;
	int maxX = 0;
	int maxY = 0;
	int scrollStepX = 0;
	int scrollStepY = 0;
	bool horizontalScrollEnabled = true;
	bool verticalScrollEnabled = true;
	sf::Vector2u areaSize;
	bool m_repaint = false;
	constexpr static float step = 0.1f;
	constexpr static unsigned scrollBarSize = 8;

	void initArea();
	void updateArea();
};

#endif // ARCSCROLLAREA_H
