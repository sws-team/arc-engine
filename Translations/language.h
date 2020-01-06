#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "stdheader.h"

class Language
{
public:
	static Language &Instance();

	enum TR_TEXT
	{
		LANGUAGE_NAME,

		//menu
		CAMPAIGN,
		OPTIONS,
		CREDITS,
		EXIT,

		//settings
		SETTINGS,
		AUDIO,
		SOUND,
		MUSIC,
		VIDEO,
		FULLSCREEN,
		RESOLUTION,
		MISC,
		LANGUAGE,
		ACCEPT,
		CANCEL,

		//panel
		ABILITY_VENOM,
		ABILITY_BOMB,
		ABILITY_FREEZE_BOMB,
		ABILITY_INCREASE_TOWER_ATTACK_SPEED,
		ABILITY_INCREASE_TOWER_DAMAGE,
		ABILITY_STOP,
		SELL,
		UPGRADE,
		TOWER_BASE,
		TOWER_POWER,
		TOWER_ROCKET,
		TOWER_FREEZE,
		TOWER_LASER,
		TOWER_IMPROVED,

		LEVEL,
		DAMAGE_PER_SECOND,
		RADIUS,
		KILLS,
		COST,
		SELL_COST,

		BASE_TOWER_DESCRIPTION,
		FREEZE_TOWER_DESCRIPTION,
		POWER_TOWER_DESCRIPTION,
		ROCKET_TOWER_DESCRIPTION,
		LASER_TOWER_DESCRIPTION,
		IMPROVED_TOWER_DESCRIPTION,

		BOMB_ABILITY_DESCRIPTION,
		FREEZE_BOMB_ABILITY_DESCRIPTION,
		VENOM_ABILITY_DESCRIPTION,
		INC_DMG_ABILITY_DESCRIPTION,
		INC_AS_ABILITY_DESCRIPTION,
		STOP_ABILITY_DESCRIPTION,

		//Game
		PAUSED,
		CONTINUE,
		RESTART,
		EXIT_TO_MENU,
		EXIT_FROM_GAME,

		GAME_OVER,
		CONGRATULATIONS,
		START_GAME,

		//Instructions
		INSTRUCTION_WELCOME,
		INSTRUCTION_TOWERS,
		INSTRUCTION_ABILITIES,
		INSTRUCTION_MONEY,
		INSTRUCTION_HEALTH,
		INSTRUCTION_REMOVE,
		INSTRUCTION_UPGRADE,
		INSTRUCTION_PROGRESS,
		INSTRUCTION_SKIP,
	};

	wstring translate(TR_TEXT id) const;
	wstring translate(int id) const;

	vector<wstring> getAvaliableLanguageNames() const;
	void setCurrentLanguage(const string& lang);
	void setCurrentLanguageByName(const wstring& name);
	wstring getCurrentLanguageName() const;
	void load();

private:
	Language();

	map<string, map<TR_TEXT, wstring> > langs;
	string currentLangName;

	void loadRus();
	void loadEng();
	void loadFr();
};

#endif // LANGUAGE_H
