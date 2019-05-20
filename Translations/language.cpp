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

	lang.insert(pair<TR_TEXT, wstring>(PLAY, L"Играть"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Опции"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"О программе"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Выход"));

	lang.insert(pair<TR_TEXT, wstring>(DAMAGE, L"Урон"));
	lang.insert(pair<TR_TEXT, wstring>(ATTACK_SPEED, L"Скорость атаки"));
	lang.insert(pair<TR_TEXT, wstring>(CRIT_CHANCE, L"Шанс крита"));
	lang.insert(pair<TR_TEXT, wstring>(CRIT_DAMAGE, L"Урон крита"));
	lang.insert(pair<TR_TEXT, wstring>(REFLECTION, L"Поглощение"));
	lang.insert(pair<TR_TEXT, wstring>(HEALTH, L"Здоровье"));
	lang.insert(pair<TR_TEXT, wstring>(ENERGY, L"Энергия"));
	lang.insert(pair<TR_TEXT, wstring>(SHIELD, L"Щиты"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_COST, L"Стоимость абилки"));
	lang.insert(pair<TR_TEXT, wstring>(PLAYERS, L"Ироки"));
	lang.insert(pair<TR_TEXT, wstring>(STATS, L"Хар-ки"));
	lang.insert(pair<TR_TEXT, wstring>(WEAPONS, L"Оружие"));
	lang.insert(pair<TR_TEXT, wstring>(ARTIFACTS, L"Артефакты"));
	lang.insert(pair<TR_TEXT, wstring>(INFORMATION, L"Информация"));
	lang.insert(pair<TR_TEXT, wstring>(GEARS, L"Механизмы"));
	lang.insert(pair<TR_TEXT, wstring>(POINTS, L"Очки"));
	lang.insert(pair<TR_TEXT, wstring>(CHARACTERS, L"Персонажи"));
	lang.insert(pair<TR_TEXT, wstring>(SPACESHIP, L"Звездолёт"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(rus, lang));
}

void Language::loadEng()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"English(UK)"));

	lang.insert(pair<TR_TEXT, wstring>(PLAY, L"Play"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Options"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"Credits"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Exit"));

	lang.insert(pair<TR_TEXT, wstring>(DAMAGE, L"Damage"));
	lang.insert(pair<TR_TEXT, wstring>(ATTACK_SPEED, L"Attack speed"));
	lang.insert(pair<TR_TEXT, wstring>(CRIT_CHANCE, L"Crit chance"));
	lang.insert(pair<TR_TEXT, wstring>(CRIT_DAMAGE, L"Crit damage"));
	lang.insert(pair<TR_TEXT, wstring>(REFLECTION, L"Reflection"));
	lang.insert(pair<TR_TEXT, wstring>(HEALTH, L"Health"));
	lang.insert(pair<TR_TEXT, wstring>(ENERGY, L"Energy"));
	lang.insert(pair<TR_TEXT, wstring>(SHIELD, L"Shield"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_COST, L"Ability cost"));
	lang.insert(pair<TR_TEXT, wstring>(PLAYERS, L"Players"));
	lang.insert(pair<TR_TEXT, wstring>(STATS, L"Stats"));
	lang.insert(pair<TR_TEXT, wstring>(WEAPONS, L"Weapons"));
	lang.insert(pair<TR_TEXT, wstring>(ARTIFACTS, L"Artifacts"));
	lang.insert(pair<TR_TEXT, wstring>(INFORMATION, L"Info"));
	lang.insert(pair<TR_TEXT, wstring>(GEARS, L"Gears"));
	lang.insert(pair<TR_TEXT, wstring>(POINTS, L"Points"));
	lang.insert(pair<TR_TEXT, wstring>(CHARACTERS, L"Characters"));
	lang.insert(pair<TR_TEXT, wstring>(SPACESHIP, L"Spaceship"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(eng, lang));
}

void Language::loadFr()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"French"));

	lang.insert(pair<TR_TEXT, wstring>(PLAY, L"Jouer"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Options"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"Crédits"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Sortir du jeu"));

	lang.insert(pair<TR_TEXT, wstring>(DAMAGE, L"Damage"));
	lang.insert(pair<TR_TEXT, wstring>(ATTACK_SPEED, L"Vitesse d'attaque"));
	lang.insert(pair<TR_TEXT, wstring>(CRIT_CHANCE, L"Crit chance"));
	lang.insert(pair<TR_TEXT, wstring>(CRIT_DAMAGE, L"Crit damage"));
	lang.insert(pair<TR_TEXT, wstring>(REFLECTION, L"Reflection"));
	lang.insert(pair<TR_TEXT, wstring>(HEALTH, L"Vie"));
	lang.insert(pair<TR_TEXT, wstring>(ENERGY, L"Energie"));
	lang.insert(pair<TR_TEXT, wstring>(SHIELD, L"Ecran"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_COST, L"Coût de force"));
	lang.insert(pair<TR_TEXT, wstring>(PLAYERS, L"Joueur"));
	lang.insert(pair<TR_TEXT, wstring>(STATS, L"Statistiques"));
	lang.insert(pair<TR_TEXT, wstring>(WEAPONS, L"Armures"));
	lang.insert(pair<TR_TEXT, wstring>(ARTIFACTS, L"Artefacts"));
	lang.insert(pair<TR_TEXT, wstring>(INFORMATION, L"Info"));
	lang.insert(pair<TR_TEXT, wstring>(GEARS, L"Gears"));
	lang.insert(pair<TR_TEXT, wstring>(POINTS, L"Points"));
	lang.insert(pair<TR_TEXT, wstring>(CHARACTERS, L"Personnages"));
	lang.insert(pair<TR_TEXT, wstring>(SPACESHIP, L"Vaisseau"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(fr, lang));
}

vector<pair<wstring, string> > Language::getAvaliableLanguages() const
{
	vector<pair<wstring, string> > langsNames;
	for(const auto& it : langs)
	{
		pair<wstring, string> languagePair;
		languagePair.first = it.second.at(LANGUAGE_NAME);
		languagePair.second = it.first;
		langsNames.push_back(languagePair);
	}
	return langsNames;
}

void Language::setCurrentLanguage(const string &lang)
{
	currentLangName = lang;
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
