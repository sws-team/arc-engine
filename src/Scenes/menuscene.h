#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <arcscene.h>

class MenuScene : public ArcScene
{
public:
	MenuScene(const std::string& name = "MenuScene");

	void init() override;
	bool eventFilter(sf::Event* event) override;

	void addMenu(ArcButton* menu);

	void setMenuPos(const sf::Vector2f &pos);
	void setMenuPos(float x, float y);

protected:
	void accept();
	int currentMenu = 0;
	ArcLayout *menuLayout = nullptr;
	std::vector<ArcButton*> menus;
private:
	void menuUp();
	void menuDown();
	static constexpr float DEFAULT_Y_OFFSET = 30;
};

#endif // MENUSCENE_H
