#include "language.h"
#include "json/json.h"

const string eng = "eng";
const string rus = "rus";
const string fr = "fr";

Language &Language::Instance()
{
	static Language instance;
	return instance;
}

Language::Language()
{
	setCurrentLanguage(rus);
}

void Language::loadRus()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"Русский"));

	//menu
	lang.insert(pair<TR_TEXT, wstring>(CAMPAIGN, L"Кампания"));
	lang.insert(pair<TR_TEXT, wstring>(SURVIVAL_MODE, L"Режим выживания"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Опции"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"О программе"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Выход"));

	//settings
	lang.insert(pair<TR_TEXT, wstring>(SETTINGS, L"Настройки"));
	lang.insert(pair<TR_TEXT, wstring>(AUDIO, L"Аудио"));
	lang.insert(pair<TR_TEXT, wstring>(SOUND, L"Звуки"));
	lang.insert(pair<TR_TEXT, wstring>(MUSIC, L"Музыка"));
	lang.insert(pair<TR_TEXT, wstring>(VIDEO, L"Видео"));
	lang.insert(pair<TR_TEXT, wstring>(FULLSCREEN, L"Полноэкранный режим"));
	lang.insert(pair<TR_TEXT, wstring>(RESOLUTION, L"Разрешение"));
	lang.insert(pair<TR_TEXT, wstring>(MISC, L"Разное"));
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE, L"Язык"));
	lang.insert(pair<TR_TEXT, wstring>(ACCEPT, L"Принять"));
	lang.insert(pair<TR_TEXT, wstring>(CANCEL, L"Отмена"));

	//panel
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_VENOM, L"Venom"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_BOMB, L"Bomb"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"ic tower att speed"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_DAMAGE, L"inc tower dmg"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_UNKNOWN, L"unknown"));
	lang.insert(pair<TR_TEXT, wstring>(SELL, L"Sell"));
	lang.insert(pair<TR_TEXT, wstring>(UPGRADE, L"Updgrade"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_BASE, L"Tower base"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_POWER, L"Tower power"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_ROCKET, L"Tower rocket"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_FREEZE, L"Tower freeze"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_LASER, L"Tower laser"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_IMPROVED, L"Tower improved"));
	lang.insert(pair<TR_TEXT, wstring>(LEVEL, L"Level"));
	lang.insert(pair<TR_TEXT, wstring>(DAMAGE, L"Damage"));
	lang.insert(pair<TR_TEXT, wstring>(ATTACK_SPEED, L"Attack speed"));
	lang.insert(pair<TR_TEXT, wstring>(RADIUS, L"Radius"));
	lang.insert(pair<TR_TEXT, wstring>(PROJECTILE_SPEED, L"Projectile speed"));
	lang.insert(pair<TR_TEXT, wstring>(COST, L"Cost"));
	lang.insert(pair<TR_TEXT, wstring>(SELL_COST, L"Sell cost"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(rus, lang));
}

void Language::loadEng()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"English(UK)"));

	//menu
	lang.insert(pair<TR_TEXT, wstring>(CAMPAIGN, L"Play"));
	lang.insert(pair<TR_TEXT, wstring>(SURVIVAL_MODE, L"Survival mode"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Options"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"Credits"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Exit"));

	//settings
	lang.insert(pair<TR_TEXT, wstring>(SETTINGS, L"Settings"));
	lang.insert(pair<TR_TEXT, wstring>(AUDIO, L"Audio"));
	lang.insert(pair<TR_TEXT, wstring>(SOUND, L"Sound"));
	lang.insert(pair<TR_TEXT, wstring>(MUSIC, L"Music"));
	lang.insert(pair<TR_TEXT, wstring>(VIDEO, L"Video"));
	lang.insert(pair<TR_TEXT, wstring>(FULLSCREEN, L"Fullscreen"));
	lang.insert(pair<TR_TEXT, wstring>(RESOLUTION, L"Resolution"));
	lang.insert(pair<TR_TEXT, wstring>(MISC, L"Misc"));
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE, L"Language"));
	lang.insert(pair<TR_TEXT, wstring>(ACCEPT, L"Accept"));
	lang.insert(pair<TR_TEXT, wstring>(CANCEL, L"Cancel"));

	//panel
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_VENOM, L"Venom"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_BOMB, L"Bomb"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"ic tower att speed"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_DAMAGE, L"inc tower dmg"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_UNKNOWN, L"unknown"));
	lang.insert(pair<TR_TEXT, wstring>(SELL, L"Sell"));
	lang.insert(pair<TR_TEXT, wstring>(UPGRADE, L"Updgrade"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_BASE, L"Tower base"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_POWER, L"Tower power"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_ROCKET, L"Tower rocket"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_FREEZE, L"Tower freeze"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_LASER, L"Tower laser"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_IMPROVED, L"Tower improved"));
	lang.insert(pair<TR_TEXT, wstring>(LEVEL, L"Level"));
	lang.insert(pair<TR_TEXT, wstring>(DAMAGE, L"Damage"));
	lang.insert(pair<TR_TEXT, wstring>(ATTACK_SPEED, L"Attack speed"));
	lang.insert(pair<TR_TEXT, wstring>(RADIUS, L"Radius"));
	lang.insert(pair<TR_TEXT, wstring>(PROJECTILE_SPEED, L"Projectile speed"));
	lang.insert(pair<TR_TEXT, wstring>(COST, L"Cost"));
	lang.insert(pair<TR_TEXT, wstring>(SELL_COST, L"Sell cost"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(eng, lang));
}

void Language::loadFr()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"French"));

	//menu
	lang.insert(pair<TR_TEXT, wstring>(CAMPAIGN, L"Jouer"));
	lang.insert(pair<TR_TEXT, wstring>(SURVIVAL_MODE, L"Survival mode"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Options"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"Crédits"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Sortir du jeu"));

	//settings
	lang.insert(pair<TR_TEXT, wstring>(SETTINGS, L"Settings"));
	lang.insert(pair<TR_TEXT, wstring>(AUDIO, L"Audio"));
	lang.insert(pair<TR_TEXT, wstring>(SOUND, L"Sound"));
	lang.insert(pair<TR_TEXT, wstring>(MUSIC, L"Music"));
	lang.insert(pair<TR_TEXT, wstring>(VIDEO, L"Video"));
	lang.insert(pair<TR_TEXT, wstring>(FULLSCREEN, L"Fullscreen"));
	lang.insert(pair<TR_TEXT, wstring>(RESOLUTION, L"Resolution"));
	lang.insert(pair<TR_TEXT, wstring>(MISC, L"Misc"));
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE, L"Language"));
	lang.insert(pair<TR_TEXT, wstring>(ACCEPT, L"Accept"));
	lang.insert(pair<TR_TEXT, wstring>(CANCEL, L"Cancel"));

	//panel
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_VENOM, L"Venom"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_BOMB, L"Bomb"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"ic tower att speed"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_DAMAGE, L"inc tower dmg"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_UNKNOWN, L"unknown"));
	lang.insert(pair<TR_TEXT, wstring>(SELL, L"Sell"));
	lang.insert(pair<TR_TEXT, wstring>(UPGRADE, L"Updgrade"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_BASE, L"Tower base"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_POWER, L"Tower power"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_ROCKET, L"Tower rocket"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_FREEZE, L"Tower freeze"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_LASER, L"Tower laser"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_IMPROVED, L"Tower improved"));
	lang.insert(pair<TR_TEXT, wstring>(LEVEL, L"Level"));
	lang.insert(pair<TR_TEXT, wstring>(DAMAGE, L"Damage"));
	lang.insert(pair<TR_TEXT, wstring>(ATTACK_SPEED, L"Attack speed"));
	lang.insert(pair<TR_TEXT, wstring>(RADIUS, L"Radius"));
	lang.insert(pair<TR_TEXT, wstring>(PROJECTILE_SPEED, L"Projectile speed"));
	lang.insert(pair<TR_TEXT, wstring>(COST, L"Cost"));
	lang.insert(pair<TR_TEXT, wstring>(SELL_COST, L"Sell cost"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(fr, lang));
}

vector<wstring> Language::getAvaliableLanguageNames() const
{
	vector<wstring> langsNames;
	for(const auto& it : langs)
		langsNames.push_back(it.second.at(LANGUAGE_NAME));
	return langsNames;
}

void Language::setCurrentLanguage(const string &lang)
{
	currentLangName = lang;
}

void Language::setCurrentLanguageByName(const wstring &name)
{
	for(const auto& it : langs)
	{
		if (it.second.at(LANGUAGE_NAME) == name)
		{
			setCurrentLanguage(it.first);
			break;
		}
	}
}

wstring Language::getCurrentLanguageName() const
{
	return langs.at(currentLangName).at(LANGUAGE_NAME);
}

void Language::load()
{
	loadEng();
	loadRus();
	loadFr();
}

wstring Language::translate(TR_TEXT id) const
{
	return langs.at(currentLangName).at(id);
}

wstring Language::translate(int id) const
{
	return translate(static_cast<TR_TEXT>(id));
}
