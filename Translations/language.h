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
		PLAY,
		OPTIONS,
		CREDITS,
		EXIT,

		//settings
		SOUND,
		MUSIC,
		FULLSCREEN,
		RESOLUTION,
		LANGUAGE,
		ACCEPT,
		CANCEL,
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
