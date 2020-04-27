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
		DRAIN
	};

	static sf::String towerInfoText(TOWER_TYPES type);
	static sf::String abilityInfoText(ACTION_STATE type);
	static sf::String mapEffectInfoText(MAP_EFFECTS type);

private:
	sf::RectangleShape shadowRect;

	enum STATES
	{
		WELCOME,

		MONEY,
		HEALTH,
		PROGRESS,

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

		FINISHED
	};
	void updateState();

	sf::Text text;
	sf::RectangleShape targetRect;

	constexpr static int INSTRUCTIONS_OFFSET = 350;

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
	void updateArrowPos();
	static constexpr float TEXT_WIDTH = 966;
	static constexpr float RECT_OFFSET = 5;
	static constexpr float RUBRIC_OFFSET = 16;
};

#endif // INSTRUCTIONS_H
