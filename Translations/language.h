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
		PLAY,
		OPTIONS,
		CREDITS,
		EXIT,

		DAMAGE,
		ATTACK_SPEED,
		CRIT_CHANCE,
		CRIT_DAMAGE,
		REFLECTION,
		HEALTH,
		ENERGY,
		SHIELD,
		ABILITY_COST,

		PLAYERS,
		STATS,
		WEAPONS,
		ARTIFACTS,
		INFORMATION,
		GEARS,
		POINTS,
		CHARACTERS,
		SPACESHIP,

	};

	wstring translate(TR_TEXT id) const;
	wstring translate(int id) const;

	vector<pair<wstring, string> > getAvaliableLanguages() const;
	void setCurrentLanguage(const string& lang);
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
