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
		SURVIVAL_MODE,
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
		ABILITY_UNKNOWN,
		SELL,
		UPGRADE,
		TOWER_BASE,
		TOWER_POWER,
		TOWER_ROCKET,
		TOWER_FREEZE,
		TOWER_LASER,
		TOWER_IMPROVED,

		LEVEL,
		DAMAGE,
		ATTACK_SPEED,
		RADIUS,
		PROJECTILE_SPEED,
		COST,
		SELL_COST,

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
		INSTRUCTION_ENERGY,
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
