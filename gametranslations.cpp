#include "gamemanagers.h"
#include "engine.h"

#define TR_PAIR(X,Y) std::pair<int, sf::String>(X, Y)

void GameManagers::loadTranslations()
{
	std::map<int, sf::String> russainTranslation;
	std::map<int, sf::String> englishTranslation;
	std::map<int, sf::String> frenchTranslation;

	russainTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"Русский"));
	englishTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"French"));

	//menu
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Play"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Играть"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Jouer"));

	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Опции"));
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Options"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Справочник"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));

	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Выход"));
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Exit"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Sortir du jeu"));

	//manual
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Speed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Скорость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Vitesse"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Size"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Размер"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Taille"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Armour"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Броня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Blindage"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Health points"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Points de vie"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Abilities"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Pouvoirs"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Small"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Мелкий"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Petite"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Mid"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Средний"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Moyenne"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Big"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Крупный"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Grande"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Slow"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Медленный"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Petite"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normale"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Fast"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Быстрая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Grande"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Next"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Вперед"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Suivant"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Previous"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Précédent"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Credits"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Авторы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Auteurs"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Back"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Вернуться"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Revenir"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Light"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Легкая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Léger"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Medium"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Средняя"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Moyen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Strong"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Крепкая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Épais"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Cooldown time"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Время перезарядки"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Temps de rechargement"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Урон"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Время действия"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));

	//settings
	englishTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Settings"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Настройки"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Parametres"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Sound"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Звуки"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Sons"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Music"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Музыка"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Musique"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Fullscreen"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Полноэкранный режим"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Plein ecran"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Resolution"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Разрешение"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Resolution"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Language"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Язык"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Langue"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Accept"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Принять"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Confirmer"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Cancel"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Отмена"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Supprimer"));

	//game
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Paused"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Пауза"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Pause"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Continue"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Продолжить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Continuer"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Restart"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Рестарт"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Recommencer"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Exit to menu"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Выход в меню"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Sortir au menu"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Exit from game"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Выход из игры"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Sortir du jeu"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"You lose!\nTry again."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Вы проиграли!.\nПопробуйте еще."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Désolé, t'as pérdu, on recommense?"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations!\nLevel completed."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Поздравляем!\nУровень пройден."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Cool! T'as réussi!"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Press Space to start"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Нажмите Пробел чтобы начать"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Tapez l'Espace afin de commencer le jeu"));

	//instructions
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Hello there! Welcome to Arc Defence game.\nWe really need your help here.\nI’ll get you acquaintance with details."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Добро пожаловать в игру Arc Defence.\nНам нужна твоя помощь в защите территории.\nЯ введу тебя в курс дела."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Bonjour, mon pote! T’es maintenant au monde d’Arc Defence.\nNous avons vraiment besoin de ton aide.\nJe vais te renseigner comment on se débrouille là."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Energy\nEnergy is crucial for us.\nWe can extract a big amount of his resource due to\nEnergy tower. It is also possible to gain a bit of energy\nby exterminating enemies."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Энергия\nЭто наш основной ресурс.\nВ большом количестве ее можно добыть благодаря\nЭнергетической башне. Уничтожение врага тоже немного\nпополнит запас энергии."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Énergie\nElle est essentielle pour nous.\nOn en extrait en grande quantité grâce aux Tours de production d'énergie. On peut également en obtenir en peu après avoir tuer un ennemi."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health scale\nWe can’t let them go further! Only a little amount of enemies\ncan cross the line without endanger our defensive campaign.\nEvery single enemy crossing the finish line takes up some\nhealth points from you: the bigger enemy is,\nthe more points are taken."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Шкала жизни\nМы не можем допустить, чтобы большое количество врагов\nперешло рубеж. Каждый враг, которому удается от нас уйти,\nуменьшает показатели шкалы на определенное количество очков.\nОбрати внимание, количество очков зависит от того,\nнасколько они опасны: небольшие не причиняют большого вреда,\nа мощным необходимо оказать решительный отпор."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Points de vie\nOn ne doit absolument pas laisser les ennemis passer plus loin. L’échelle des points de vie indique la quantité des ennemis qui peuvent passer la frontière sans représenter l’ultime menace. S’il ne reste plus de points de vie, on est foutus. Des ennemis géants sont plus dangereux!"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress\nOur enemies tend to hit in waves, fortunately, we can predict\napproximate number of waves, you’ll see it here."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Прогресс уровня.\nОбычно наши враги нападают волнами. Мы можем предсказать их\nпримерное количество и вывести тебе на шкалу."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Avancement\nAfin d’attaquer les ennemis forment une sorte des vagues. Nous pouvons prévoir le nombre des vagues, cette échelle sert à l’indiquer pour toi."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"So it’s time for you to show us what you can do!\nGood luck!"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"А теперь покажи нам, на что способен!\nУдачи!"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"Bon, maintenant c’est à toi d’agir!\nBonne chance!"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Нажмите Ввод чтобы продолжить или Пробел чтобы пропустить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Taper la touche Entree pour continuer ou l'Espace pour ignorer"));

	//enemies
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infantry squad"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Отряд пехоты"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infanterie"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"War vehicle \"Praefectus\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Боевая машина \"Префект\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Véhicule de combat \"Préfet\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Recon tricycle \"IS2\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Разведывательный трицикл \"ИС2\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Tricycle de reconnaissance \"IS2\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Tank \"W-100\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Танк \"В-100\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Char \"W-100\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Паук"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Мехпаук"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"MechAraignée"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Туатара"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Giant slug"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Гигантский слизняк"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Limace géante"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Heavy tank \"Juggernaut\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Тяжелый танк \"Джаггернаут\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Char lourd \"Juggernaut\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Sluggy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Слизнячок"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Petit limace"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"ARV \"Immunes\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"БРЭМ \"Иммун\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"ARV \"Immune\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Defense combat vehicle \"Tachyon\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Оборонительная боевая машина \"Тахион\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Véhicule de combat à écran de défence \"Tachyon\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Walker \"33-Qc. rev02.9\""));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Шагоход \"33-Кц. рев02.9\""));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Marcheur \"33-Qc. rev02.9\""));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"SPG-4000"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"САУ-4000"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"Automoteur d'artillerie-4000"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larva"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Личинка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larve"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Bugsaurus"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Жукозавр"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Insectosaure"));
#ifdef WITH_DIFFICULT
	//difficult
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EASY, L"Easy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EASY, L"Легко"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EASY, L"Easy"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL, L"Normal"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL, L"Нормально"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL, L"Normal"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HARD, L"Hard"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HARD, L"Трудно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HARD, L"Hard"));
#endif
	//towers abilities
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnerability"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Неуязвимость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnérabilité"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Freeze on area"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Заморозка по площади"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Congélation d'une zone"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Периодично испускает энергетические импульсы,\n наносящие урон врагам рядом"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Burn enemy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Поджигает врага"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Несколько целей"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Shoot on 4 sides"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Стрельба на 4 стороны"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Shoot on 4 sides"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Price"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Цена за постройки увеличивается\nс каждой построенное башней"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"???"));

	//enemies abilities
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Отсутствует"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Rage - "));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Ярость - броня увеличивается\nобратно пропорционально здоровью"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Spawn"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Может родить новых личинок"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Телепорт"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"shell"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Генератор силового поля\nувеличивает броню окружающих"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Heal"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Ремонтное оборудование\nпозволяет восстанавливать некоторое\nколичество здоровья"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Может выпускать паутину из наноботов,\nкоторая временно выводит из строя башню"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Оснащен энергетическим щитом,\nвеличина заряда которого зависит от времени"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Оснащен излучателем направленного действия,\nкоторый может вызывать поломки башен"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Усиленная регенерация\nпозволяет восстанавливать здоровье"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Ускоряется пропорционально\nотнятому здоровью"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Может уничтожить башню,\nвыстрелив в нее"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Может ухудшить башню,\nвыстрелив в нее"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"???"));

	//panel
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Acid"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Кислота"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Acide"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bomb"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Бомба"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Bombe-glace"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Ускорение башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour a vitesse d'attaque augmentee"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Усиление башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages augmentes"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stop all"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Остановка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stase"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Base tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Обычная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Tour de base"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Energy tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Энергетическая башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Tour de production d'energie"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Rocket tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Ракетная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Tour reacteur-fusee"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Tower freeze"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Замораживающая башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Tour a obus glacants"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Tower laser"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Лазерная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Tour laser"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Tower improved"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Улучшенная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Tour de base amelioree"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Level"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Уровень"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Niveau"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Damage per seconds"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Урон в секунду"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Dommages par seconde"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Radius"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Радиус"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Rayon"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Kills"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Фраги"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Ennemis tues"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Cost"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Цена"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Prix"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Sell cost"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Цена продажи"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Prix de la vente"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Wave"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Волна"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Vague"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limit"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Лимит"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limite"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Has a cumulative effect of reducing enemy armor"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Effet cumulatif de reduire la cuirasse d'un ennemi"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Produces resources"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Производит ресурсы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Extraire les ressourses"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон\nпо площади"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Dommage d'une certaine territoire"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем\nпоказателям"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Exploision instantanee"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantane d'un ennemi"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Непрерывно наносит урон\nв области"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Dommages prolonge d'une certaine territoire"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон\nбашни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Amelioration temporaire des dommages causes"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Acceleration temporaire des tirs"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Останавливает движение всех\nврагов"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));

	//map effects
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Mist"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Туман"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Energy leech"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Энергопиявка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"???"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"ICBM"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"МБР"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"При попадании на башню ограничивет\nточность приборов наведения.\nУменьшает радиус стрельбы башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Выкачивает энергию из энергетических башен.\nМожет быть уничтожен взрывом бомбы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Вызывает помехи в работе башен,\nснижается урон и скорость стрельбы."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"???"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"???"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Уничтожает башню независимо от уровня."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"???"));

	Engine::Instance().translationsManager()->addTranslation(std::string("russian"), russainTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("english"), englishTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("french"), frenchTranslation);
}
