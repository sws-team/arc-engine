#include "language.h"

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
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Опции"));
	lang.insert(pair<TR_TEXT, wstring>(MANUAL, L"Справочник"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"Авторы"));
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
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_VENOM, L"Кислота"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_BOMB, L"Бомба"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Ускорение башни"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_DAMAGE, L"Увеличение урона башни"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_STOP, L"Остановка"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_BASE, L"Обычная башня"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_POWER, L"Энергетическая башня"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_ROCKET, L"Ракетная башня"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_FREEZE, L"Замораживающая башня"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_LASER, L"Лазерная башня"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_IMPROVED, L"Улучшенная башня"));
	lang.insert(pair<TR_TEXT, wstring>(LEVEL, L"Уровень"));
	lang.insert(pair<TR_TEXT, wstring>(DAMAGE_PER_SECOND, L"Урон в секунду"));
	lang.insert(pair<TR_TEXT, wstring>(RADIUS, L"Радиус"));
	lang.insert(pair<TR_TEXT, wstring>(KILLS, L"Фраги"));
	lang.insert(pair<TR_TEXT, wstring>(COST, L"Цена"));
	lang.insert(pair<TR_TEXT, wstring>(SELL_COST, L"Цена продажи"));
	lang.insert(pair<TR_TEXT, wstring>(WAVE, L"Волна"));
	lang.insert(pair<TR_TEXT, wstring>(BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	lang.insert(pair<TR_TEXT, wstring>(FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	lang.insert(pair<TR_TEXT, wstring>(POWER_TOWER_DESCRIPTION, L"Производит ресурсы"));
	lang.insert(pair<TR_TEXT, wstring>(ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон\nпо площади"));
	lang.insert(pair<TR_TEXT, wstring>(LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	lang.insert(pair<TR_TEXT, wstring>(IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем\nпоказателям"));
	lang.insert(pair<TR_TEXT, wstring>(BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	lang.insert(pair<TR_TEXT, wstring>(FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	lang.insert(pair<TR_TEXT, wstring>(VENOM_ABILITY_DESCRIPTION, L"Непрерывно наносит урон\nв области"));
	lang.insert(pair<TR_TEXT, wstring>(INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон\nбашни"));
	lang.insert(pair<TR_TEXT, wstring>(INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	lang.insert(pair<TR_TEXT, wstring>(STOP_ABILITY_DESCRIPTION, L"Останавливает движение всех\nврагов"));

	//game
	lang.insert(pair<TR_TEXT, wstring>(PAUSED, L"Пауза"));
	lang.insert(pair<TR_TEXT, wstring>(CONTINUE, L"Продолжить"));
	lang.insert(pair<TR_TEXT, wstring>(RESTART, L"Рестарт"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT_TO_MENU, L"Выход в меню"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT_FROM_GAME, L"Выход из игры"));
	lang.insert(pair<TR_TEXT, wstring>(GAME_OVER, L"Игра окончена"));
	lang.insert(pair<TR_TEXT, wstring>(CONGRATULATIONS, L"Поздравляем"));
	lang.insert(pair<TR_TEXT, wstring>(START_GAME, L"Нажмите Пробел чтобы начать"));

	//instructions
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_WELCOME, L"Добро пожаловать!"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_TOWERS, L"Башни"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_ABILITIES, L"Способности"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_MONEY, L"Валюта"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_HEALTH, L"Здоровье"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_PROGRESS, L"Прогресс уровня"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_SKIP, L"Нажмите Ввод чтобы продолжить или Пробел чтобы пропустить"));
	langs.insert(pair<string, map<TR_TEXT, wstring> >(rus, lang));
}

void Language::loadEng()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"English"));

	//menu
	lang.insert(pair<TR_TEXT, wstring>(CAMPAIGN, L"Play"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Options"));
	lang.insert(pair<TR_TEXT, wstring>(MANUAL, L"Manual"));
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
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_STOP, L"Stop all"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_BASE, L"Tower base"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_POWER, L"Tower power"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_ROCKET, L"Tower rocket"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_FREEZE, L"Tower freeze"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_LASER, L"Tower laser"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_IMPROVED, L"Tower improved"));
	lang.insert(pair<TR_TEXT, wstring>(LEVEL, L"Level"));
	lang.insert(pair<TR_TEXT, wstring>(DAMAGE_PER_SECOND, L"Damage per seconds"));
	lang.insert(pair<TR_TEXT, wstring>(RADIUS, L"Radius"));
	lang.insert(pair<TR_TEXT, wstring>(KILLS, L"Kills"));
	lang.insert(pair<TR_TEXT, wstring>(COST, L"Cost"));
	lang.insert(pair<TR_TEXT, wstring>(SELL_COST, L"Sell cost"));
	lang.insert(pair<TR_TEXT, wstring>(WAVE, L"Wave"));
	lang.insert(pair<TR_TEXT, wstring>(BASE_TOWER_DESCRIPTION, L"Has a cumulative effect of reducing enemy armor"));
	lang.insert(pair<TR_TEXT, wstring>(FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	lang.insert(pair<TR_TEXT, wstring>(POWER_TOWER_DESCRIPTION, L"Produces resources"));
	lang.insert(pair<TR_TEXT, wstring>(ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	lang.insert(pair<TR_TEXT, wstring>(LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	lang.insert(pair<TR_TEXT, wstring>(IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	lang.insert(pair<TR_TEXT, wstring>(BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	lang.insert(pair<TR_TEXT, wstring>(FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	lang.insert(pair<TR_TEXT, wstring>(VENOM_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	lang.insert(pair<TR_TEXT, wstring>(INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	lang.insert(pair<TR_TEXT, wstring>(INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	lang.insert(pair<TR_TEXT, wstring>(STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));

	//game
	lang.insert(pair<TR_TEXT, wstring>(PAUSED, L"Paused"));
	lang.insert(pair<TR_TEXT, wstring>(CONTINUE, L"Continue"));
	lang.insert(pair<TR_TEXT, wstring>(RESTART, L"Restart"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT_TO_MENU, L"Exit to menu"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT_FROM_GAME, L"Exit from game"));
	lang.insert(pair<TR_TEXT, wstring>(GAME_OVER, L"Game Over!"));
	lang.insert(pair<TR_TEXT, wstring>(CONGRATULATIONS, L"Congratulations!"));
	lang.insert(pair<TR_TEXT, wstring>(START_GAME, L"Press Space to start"));

	//instructions
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_WELCOME, L"Welcome!"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_TOWERS, L"Towers"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_ABILITIES, L"Abilities"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_MONEY, L"Money"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_HEALTH, L"Health"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_PROGRESS, L"Progress"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));

	langs.insert(pair<string, map<TR_TEXT, wstring> >(eng, lang));
}

void Language::loadFr()
{
	map<TR_TEXT, wstring> lang;
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE_NAME, L"French"));

	//menu
	lang.insert(pair<TR_TEXT, wstring>(CAMPAIGN, L"Jouer"));
	lang.insert(pair<TR_TEXT, wstring>(OPTIONS, L"Options"));
	lang.insert(pair<TR_TEXT, wstring>(MANUAL, L"Manual"));
	lang.insert(pair<TR_TEXT, wstring>(CREDITS, L"Crédits"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT, L"Sortir du jeu"));

	//settings
	lang.insert(pair<TR_TEXT, wstring>(SETTINGS, L"Paramètres"));
	lang.insert(pair<TR_TEXT, wstring>(AUDIO, L"Audio"));
	lang.insert(pair<TR_TEXT, wstring>(SOUND, L"Sons"));
	lang.insert(pair<TR_TEXT, wstring>(MUSIC, L"Musique"));
	lang.insert(pair<TR_TEXT, wstring>(VIDEO, L"Vidéo"));
	lang.insert(pair<TR_TEXT, wstring>(FULLSCREEN, L"Plein écran"));
	lang.insert(pair<TR_TEXT, wstring>(RESOLUTION, L"Résolution"));
	lang.insert(pair<TR_TEXT, wstring>(MISC, L"Autres"));
	lang.insert(pair<TR_TEXT, wstring>(LANGUAGE, L"Langue"));
	lang.insert(pair<TR_TEXT, wstring>(ACCEPT, L"Confirmé"));
	lang.insert(pair<TR_TEXT, wstring>(CANCEL, L"Rejet"));

	//panel
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_VENOM, L"Acide"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_BOMB, L"Bombe"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_FREEZE_BOMB, L"Bombe-glace"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour à vitesse d'attaque augmentée"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages augmentés"));
	lang.insert(pair<TR_TEXT, wstring>(ABILITY_STOP, L"Stopper"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_BASE, L"Tour de base"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_POWER, L"Tour de production d'énergie"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_ROCKET, L"Tour réacteur-fusée"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_FREEZE, L"Tour à obus glaçants"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_LASER, L"Tour laser"));
	lang.insert(pair<TR_TEXT, wstring>(TOWER_IMPROVED, L"Tour de base améliorée"));
	lang.insert(pair<TR_TEXT, wstring>(LEVEL, L"Niveau"));
	lang.insert(pair<TR_TEXT, wstring>(DAMAGE_PER_SECOND, L"Dommages"));
	lang.insert(pair<TR_TEXT, wstring>(RADIUS, L"Rayon"));
	lang.insert(pair<TR_TEXT, wstring>(KILLS, L"Ennemis tués"));
	lang.insert(pair<TR_TEXT, wstring>(COST, L"Prix"));
	lang.insert(pair<TR_TEXT, wstring>(SELL_COST, L"Prix de la vente"));
	lang.insert(pair<TR_TEXT, wstring>(WAVE, L"Vague"));
	lang.insert(pair<TR_TEXT, wstring>(BASE_TOWER_DESCRIPTION, L"Effet cumulatif de réduire la cuirasse d'un ennemi"));
	lang.insert(pair<TR_TEXT, wstring>(FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));
	lang.insert(pair<TR_TEXT, wstring>(POWER_TOWER_DESCRIPTION, L"Extraire les ressourses"));
	lang.insert(pair<TR_TEXT, wstring>(ROCKET_TOWER_DESCRIPTION, L"Dommage d'une certaine territoire"));
	lang.insert(pair<TR_TEXT, wstring>(LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));
	lang.insert(pair<TR_TEXT, wstring>(IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));
	lang.insert(pair<TR_TEXT, wstring>(BOMB_ABILITY_DESCRIPTION, L"Exploision instantanée"));
	lang.insert(pair<TR_TEXT, wstring>(FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantané d'un ennemi"));
	lang.insert(pair<TR_TEXT, wstring>(VENOM_ABILITY_DESCRIPTION, L"Dommages prolongé d'une certaine territoire"));
	lang.insert(pair<TR_TEXT, wstring>(INC_DMG_ABILITY_DESCRIPTION, L"Amélioration temporaire des dommages causés"));
	lang.insert(pair<TR_TEXT, wstring>(INC_AS_ABILITY_DESCRIPTION, L"Accélération temporaire des tirs"));
	lang.insert(pair<TR_TEXT, wstring>(STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));

	//game
	lang.insert(pair<TR_TEXT, wstring>(PAUSED, L"Pause"));
	lang.insert(pair<TR_TEXT, wstring>(CONTINUE, L"Continuer"));
	lang.insert(pair<TR_TEXT, wstring>(RESTART, L"Recommencer"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT_TO_MENU, L"Sortir au menu"));
	lang.insert(pair<TR_TEXT, wstring>(EXIT_FROM_GAME, L"Sortir du jeu"));
	lang.insert(pair<TR_TEXT, wstring>(GAME_OVER, L"Tu as perdu, désolé"));
	lang.insert(pair<TR_TEXT, wstring>(CONGRATULATIONS, L"Congratulations"));
	lang.insert(pair<TR_TEXT, wstring>(START_GAME, L"Tapez l'Espace afin de commencer le jeu"));

	//instructions
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_WELCOME, L"Welcome!"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_TOWERS, L"Towers"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_ABILITIES, L"Abilities"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_MONEY, L"Money"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_HEALTH, L"Health"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_PROGRESS, L"Progress"));
	lang.insert(pair<TR_TEXT, wstring>(INSTRUCTION_SKIP, L"Taper la Touche d'entrée pour continuer ou l'Espace pour passer"));
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
