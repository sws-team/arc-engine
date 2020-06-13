#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "stdheader.h"
#include "gameobject.h"
#include "leveldef.h"

class Instructions : public GameDrawable
{
public:
	Instructions();
	~Instructions() override;

	void init(const unsigned int level);

	void draw(sf::RenderTarget *const target) override;
	void update() override;

	bool isActive() const;
	void next();
	void skip();

	enum MAP_EFFECTS
	{
		SMOKE,
		REGRESS,
		EXPLOSION,
		DRAIN,
		LAVA,
		INVISIBILITY,
	};

	static sf::String towerInfoText(TOWER_TYPES type, bool withStats = false);
	static sf::String abilityInfoText(ACTION_STATE type);
	static sf::String mapEffectInfoText(MAP_EFFECTS type);

	static int textureRowCount(int textureId);
private:
	sf::RectangleShape shadowRect;

	enum STATES
	{
		WELCOME,

		MONEY,
		HEALTH,
		PROGRESS,
		TOWERS,
		UPGRADE_SELL,
		GOOD_LUCK,

		INSTRUCTION_TOWER_BASE,
		INSTRUCTION_TOWER_FREEZE,
		INSTRUCTION_TOWER_ENERGY,
		INSTRUCTION_TOWER_ROCKET,
		INSTRUCTION_TOWER_LASER,
		INSTRUCTION_TOWER_IMPROVED,

		INSTRUCTION_BOMB,
		INSTRUCTION_FREEZE_BOMB,
		INSTRUCTION_ACID,
		INSTRUCTION_INCREASE_ATTACK_SPEED,
		INSTRUCTION_INCREASE_DAMAGE,
		INSTRUCTION_STOP,

		MAP_SMOKE,
		MAP_REGRESS,
		MAP_EXPLOSION,
		MAP_DRAIN,
		MAP_LAVA,
		MAP_INVISIBILITY,

		FINISHED
	};
	void updateState();

	sf::Text text;
	sf::RectangleShape targetRect;

	sf::Sprite character;
	sf::RectangleShape bottom;
	sf::RectangleShape top;
	sf::RectangleShape left;
	sf::RectangleShape right;
	sf::RectangleShape textRext;
	void updateTextRect();

	static const std::map<int, std::vector<Instructions::STATES> > INSTRUCTIONS;
	std::vector<Instructions::STATES> states;
	int currentState;
	bool active;
	GameObject* arrow;
	bool showArrow;
	void updateArrowPos(const sf::Vector2f &offset);
	static constexpr float TEXT_WIDTH = 966;
	static constexpr float RECT_OFFSET = 5;
	static constexpr float RUBRIC_OFFSET = 16;
	constexpr static int INSTRUCTIONS_OFFSET = 432;
	sf::Text skipText;
	static const sf::Vector2i demoSize;
	static constexpr float DEMO_FRAME_OFFSET = 21;
	sf::RectangleShape demoRect;
	GameObject *demoObject;
};

#endif // INSTRUCTIONS_H
