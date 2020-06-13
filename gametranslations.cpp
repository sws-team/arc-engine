﻿#include "gamemanagers.h"
#include "engine.h"

#define TR_PAIR(X,Y) std::pair<int, sf::String>(X, Y)

void GameManagers::loadTranslations()
{
	std::map<int, sf::String> russainTranslation;
	std::map<int, sf::String> englishTranslation;
	std::map<int, sf::String> frenchTranslation;
	std::map<int, sf::String> germanTranslation;
	std::map<int, sf::String> chineseTranslation;

	russainTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"Русский"));
	englishTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"French"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"Deutsch"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"Chinese"));

	//menu
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Play"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Играть"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Jouer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Spiel"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"玩"));

	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Опции"));
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Options"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Optionen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"选择"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Справочник"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"菜单"));

	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Выход"));
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Exit"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Sortir du jeu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Ende"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"退出"));

	//manual
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Speed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Скорость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Vitesse"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Drehzahl"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"速度"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Size"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Размер"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Taille"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Abmessung"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"尺寸"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Armour"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Броня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Blindage"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Panzer"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"盔甲"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Health points"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Points de vie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Gesundheit"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"生命值"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Abilities"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Pouvoirs"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Fähigkeiten"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"能力"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Small"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Мелкий"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Petite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Klein"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"小"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Mid"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Средний"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Moyenne"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Mittelfein"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"中"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Big"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Крупный"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Grande"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Große"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"大"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Slow"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Медленная"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Petite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Langsam"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"慢"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normale"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"平常"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Fast"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Быстрая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Grande"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Schnell"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"快"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Next"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Вперед"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Suivant"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Nachfolgend"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"下一个"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Previous"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Précédent"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Vorausgehend"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"上一个"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Credits"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Авторы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Auteurs"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Autoren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"作者"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Back"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Вернуться"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Revenir"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Zurück"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"返回"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Light"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Легкая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Léger"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Leicht"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"光"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Medium"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Средняя"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Moyen"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Mittlere"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"媒介"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Strong"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Крепкая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Épais"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Schwere"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"强"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Cooldown time"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Время перезарядки"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Temps de rechargement"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Abfahren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"冷却时间"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Урон"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Schaden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"破坏"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Время действия"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Zeitdauer"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"持续时间"));

	//settings
	englishTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Settings"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Настройки"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Parametres"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Einstellung"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"设置"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Sound"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Звуки"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Sons"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Schall"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"声音"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Music"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Музыка"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Musique"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Musik"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"音乐"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Fullscreen"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Полноэкранный режим"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Plein ecran"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Vollbildschirm"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"全屏幕"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Resolution"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Разрешение"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Resolution"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Auflösung"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"解像度"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Language"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Язык"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Langue"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Sprache"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"语言"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Accept"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Принять"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Confirmer"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Annehmen"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"接受"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Cancel"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Отмена"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Supprimer"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Abbrechen"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"取消"));

	//game
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Paused"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Пауза"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Pause"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Pause"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"暂停"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Continue"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Продолжить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Continuer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Weiterführen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"继续"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Restart"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Рестарт"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Recommencer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Neustart"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"重启"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Exit to menu"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Выход в меню"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Sortir au menu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Zum Menü\nzurückkehren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"退出到主页"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Exit from game"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Выход из игры"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Sortir du jeu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Beende das Spiel"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"退出游戏"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"You lose!\nTry again"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Вы проиграли!\nПопробуйте еще"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Désolé, tu as pérdu la bataille, on recommense?"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Du verlierst!\nVersuch es noch einmal"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"战败。再试一次吧！"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations!\nLevel completed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Поздравляем!\nУровень пройден"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Formidable! Tu as gagné la bataille!"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Herzliche Glückwünsche!\nLevel abgeschlossen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"恭喜你，战胜了！"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Press Space to start"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Нажмите Пробел чтобы начать"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Tape l'Espace afin de commencer le jeu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Drücken Sie die Leertaste, um zu starten"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"按空白键以开始"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Press Space to skip"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Нажмите Пробел чтобы пропустить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Tape l'Espace pour ignorer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Drücken Sie die Leertaste, um zu überspringen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"按空白键跳过"));

	//instructions
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Hello there! Welcome to Arc Defence game.\nWe really need your help here.\nI’ll get you acquaintance with details."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Добро пожаловать в игру Arc Defence.\nНам нужна твоя помощь в защите территории.\nЯ введу тебя в курс дела."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Bonjour, mon pote! T’es maintenant au monde d’Arc Defence.\nNous avons vraiment besoin de ton aide.\nJe vais te renseigner comment on se débrouille là."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Hallo und willkommen bei Arc Defense.\nWir brauchen wirklich Ihre Hilfe.\nIch werde Sie mit Details vertraut machen."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"你好，欢迎来到 ARC防御，我们真的需要你的帮助，我会帮助你了解细节"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Energy\nEnergy is crucial for us.\nWe can extract a big amount of his resource due to\nEnergy tower. It is also possible to gain a bit of energy\nby exterminating enemies."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Энергия\nЭто наш основной ресурс.\nВ большом количестве ее можно добыть благодаря\nЭнергетической башне. Уничтожение врага тоже немного\nпополнит запас энергии."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Énergie\nElle est essentielle pour nous.\nOn en extrait en grande quantité grâce aux Tours de\nproduction d'énergie. On peut également en obtenir en peu\naprès avoir tuer un ennemi."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Energie.\nEs ist unsere entscheidende Ressource.\nEin großer Teil davon kann über einen Energieturm\ngewonnen werden. Es ist auch möglich,\nein bisschen Energie zu gewinnen, indem man die\nFeinde ausrottet."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"源是我们至关重要的资源。 通过能量塔可以提取大量的能量。\n也可以通过消灭敌人来获得一点能量。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health scale\nWe can’t let them go further! Only a little amount of enemies\ncan cross the line without endanger our defensive campaign.\nEvery single enemy crossing the finish line takes up some\nhealth points from you: the bigger enemy is,\nthe more points are taken."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Шкала жизни\nМы не можем допустить, чтобы большое количество врагов\nперешло рубеж. Каждый враг, которому удается от нас уйти,\nуменьшает показатели шкалы на определенное количество очков.\nОбрати внимание, что количество очков зависит от того,\nнасколько они опасны: небольшие не причиняют большого вреда,\nа мощным необходимо оказать решительный отпор."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Points de vie\nOn ne doit absolument pas laisser les ennemis passer plus loin.\nL’échelle des points de vie indique la quantité des ennemis qui\npeuvent passer la frontière sans représenter l’ultime menace.\nS’il ne reste plus de points de vie, on est foutus.\nDes ennemis géants sont plus dangereux!"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Gesundheitsskala\nWir können sie nicht weiter gehen lassen! Nur wenige Feinde\n können die Grenze überschreiten, ohne unsere\nVerteidigungskampagne zu gefährden. Jeder einzelne Feind,\nder die Ziellinie überquert, nimmt einige Gesundheitspunkte\nvon Ihnen auf: Je größer der Feind ist, desto mehr\nPunkte werden vergeben."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"生命值/ 我们不能让敌人再进一步了！ 只有一小部分敌人可以跨过防线，\n而不会危及我们的防御战。 每一个越过终点线的敌人都会从你身上攫取一些生命值:\n敌人越大，攫取的生命值就越多。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress\nOur enemies tend to hit in waves, fortunately, we can predict\napproximate number of waves, you’ll see it here."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Прогресс уровня.\nОбычно наши враги нападают волнами. Мы можем предсказать их\nпримерное количество и вывести тебе на шкалу."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Avancement\nAfin d’attaquer les ennemis forment une sorte des vagues.\nNous pouvons prévoir le nombre des vagues, cette échelle\nsert à l’indiquer pour toi."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Fortschritt\nUnsere Feinde neigen dazu, in Wellen zu schlagen.\nGlücklicherweise können wir die ungefähre Anzahl der Wellen\nvorhersagen. Sie werden sie hier sehen."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"进展/我们的敌人倾向于一波一波的攻击，幸运的是，\n我们可以预测一个大概的波数，你可以在这里看到"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Towers\nThere are two types of Towers: the Attacking towers and\nthe Energy one. You should build the Attacking towers in\nthe marked zones near the Energy ones to provide them\nnecessary resources. Upgrading an Energy tower helps\nto expand this zone."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Башни\nБашни можно разделить на Ресурсные и Атакующие.\nИз доступных нам башен только Энергетическая относится\nк первому виду. Атакующие башни необходимо строить в\nотмеченной зоне вокруг Энергетической, что позволит ей\nснабжать их необходимой энергией. При повышении уровня\nЭнергетической башни эта зона увеличивается."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Tours\nIl existe deux types des Tours: celles de\nproduction et celles d'attaque. Il est impossible de construir des\nTours d'attaque loin des Tours de production d'énergie.\nIl faut les placer dans la zone indiquée autour des Tours de\nproduction, ce qui les permettera de proqurer les Tours\nd'attaque d'énergie nécéssaire. La zone grandisse avec chaque\nupgrade de la Tour de production"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Türme\nDort die Angriffstürme und der Energieturm.\nSie sollten die Angriffstürme in markierten Zonen in der Nähe\nderEnergietürme errichten, um die erforderlichen Ressourcen\nbereitzustellen. Durch die Aufrüstung eines\nEnergieturms wird diese Zone erweitert."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"塔楼有两种: 攻击塔和能量塔。 你应该在能量塔附近的标记区域建造攻击塔，\n为他们提供必要的资源。 升级能量塔有助于扩大它周围的区域。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Tower upgrade and tower selling\nYou can improve towers characteristics by upgrading them.\nIf you ran out of energy and can't afford any upgrade,\nyou can wait until your Energy towers extract the amount\nyou need or you can sell one of your previously builded Towers.\nTo sell or upgrade a Tower you need to click on it and choose\nnecessary option."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Улучшение и продажа башен\nМожно улучшать качественные характеристики башен путем их\nапгрейда за определенное количество энергии. Если не хватает\nэнергии, подожди, пока Ресурсной башне удастся добыть нужный\nобъем, или продай башню, которая плохо справляется со своей\nзадачей. Для улучшения и продажи нажми на башню и выбери\nнеобходимый пункт."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Upgrade et vente des Tours\nTu peux améliorer les charactéristiques des Tours\ngrâce à l'upgrade. Si tu n'as pas d'énergie pour payer, tu peux\nattendre lorsque la Tour de production en extrait assez ou\nvendre une autre Tour. Pour effectuer l'upgrade ou la vente\ntu a à taper sur la Tour et choisir l'option nécéssaire."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Turm-Upgrade und Turmverkauf\nSie können die Eigenschaften von Türmen verbessern,\nindem Sie darauf klicken. Wenn Ihnen die Energie ausgeht und\nSie sich kein Upgrade leisten können, können Sie warten,\nbis Ihre Energietürme die benötigte Menge extrahiert haben,\noder Sie können einen Ihrer zuvor gebauten Türme verkaufen."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"塔的升级和出售/ 你可以通过升级塔来改变塔的特点。 如果你耗尽了能量，\n无法负担任何升级，可以等到你的能量塔提取足够的数量，\n或者你可以卖掉你以前建造的塔。 要出售或升级一个塔，你需要点击它，\n并选择必要的选项。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"So it’s time for you to show us what you can do!\nGood luck!"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"А теперь покажи нам, на что способен!\nУдачи!"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"Bon, maintenant c’est à toi d’agir!\nBonne chance!"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"Es ist Zeit für Sie, uns zu zeigen, wozu Sie fähig sind!\nViel Glück!"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"是时候让我们看看你的能耐了! 祝你好运！"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Нажмите Ввод, чтобы продолжить, или Пробел, чтобы пропустить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Tape la touche Entree pour continuer ou l'Espace pour ignorer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Drücken Sie die Eingabetaste, um fortzufahren, oder die Leertaste, um zu überspringen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"按回车键继续，或按空格键跳过"));

	//enemies
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infantry squad"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Отряд пехоты"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infanterie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infanterietrupp"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"步兵队"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"War vehicle\n«Praefectus»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Боевая машина\n«Префект»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Véhicule de combat\n«Préfet»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Kampffahrzeug\n«Präfekt»"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"praefectus战车"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Recon tricycle «IS2»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Разведывательный\nтрицикл «ИС2»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Tricycle de\nreconnaissance «IS2»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Aufklärungsdreirad IS2"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"IS2三轮侦察车"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Tank «W-100»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Танк «В-100»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Char «W-100»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Tank «W-100»"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"W-100坦克"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Mechspider"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Мехпаук"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"MechAraignée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Mechspinne"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"蜘蛛战甲"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Туатара"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"喙頭蜥"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Giant slug"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Гигантский слизняк"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Limace géante"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Riesenschnecke"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"巨蛞蝓"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Heavy tank\n«Juggernaut»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Тяжелый танк\n«Джаггернаут»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Char lourd\n«Juggernaut»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Schwerer Panzer\n«Juggernaut»"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Juggernaut重型坦克"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Sluggy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Слизнячок"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Petit limace"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Schnecklein"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"小蛞蝓"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"ARV «Immunes»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"БРЭМ «Иммун»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"ARV «Immune»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"Bergepanzer «Immun»"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"免疫 裝甲救濟車"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Defense combat\nvehicle «Tachyon»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Оборонительная боевая\nмашина «Тахион»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Véhicule à écran de\ndéfence «Tachyon»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Defensives\nKampffahrzeug «Tachyon»"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"超光速粒子防御战车"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Walker «33-Qc. rev02.9»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Шагоход «33-Кц. рев02.9»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Marcheur «33-Qc. rev02.9»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Wanderer «33-Qc. rev02.9»"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"«33-Qc. rev02.9»步行者"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Scarbreed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Шраморожденный"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Balafre-né"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Narbengeboren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"疤痕"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"SPG-4000"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"САУ-4000"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"Automoteur\nd'artillerie-4000"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"Selbstfahrlafette-4000"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"自动火炮4000型"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larva"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Личинка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larve"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larve"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"幼体"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Bugsaurus"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Жукозавр"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Insectosaure"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Käfersaurier"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"昆虫龙"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Ammonite"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Аммонит"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Ammonite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Ammoniten"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"菊石"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Acrididae"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Акрид"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Acrididae"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Acrididae"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"蝗蟲"));

	//towers abilities
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnerability"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Неуязвимость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnérabilité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Unverwundbarkeit"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"刀枪不入"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Area freeze"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Заморозка по площади"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Congélation d'une zone"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Bereich einfrieren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"区域冻结"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Periodic energy impulses,\ndamaging nearby enemies"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Периодично испускает энергетические импульсы,\nнаносящие урон врагам рядом"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Impultions d'énergie périodiques endommageant\ndes ennemis à proximité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Periodische Energieimpulse,\ndie in der Nähe befindliche Feinde beschädigen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"周期性的能量脉冲，打击附近的敌人"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Burn enemy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Поджиг врага"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Brûle un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Verbrenne den Feind"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"烧死敌人"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Несколько целей"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Mehrere Feinde anvisieren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"多目标打击"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"4-way shooting"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Стрельба на 4 стороны"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Shoot on 4 sides"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"4-Wege-Aufnahme"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"四面射击"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Building cost increases with every new tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Цена постройки увеличивается\nс каждой построенное башней"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Construction d'une chaque nouvelle\ntour augmente le prix de la suivante"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Die Baukosten steigen mit jedem neuen Turm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"每建一座新塔，建筑成本就会增加"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Can be built anywhere"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Можно построить в любом месте"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Peut être construite en tout lieu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Kann überall gebaut werden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"可以在任何地方建造"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Can be built only near energy tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Можно построить только возле\nЭнергетической башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Ne peut être construite qu'à proximité\nd'une Tour de production d'énergie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Kann nur in der Nähe des Energieturms\ngebaut werden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"只能建在能量塔附近"));

	//enemies abilities
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"None"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Отсутствует"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Absent"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Keiner"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"空缺"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Rage: The more damage taken - the stronger\nthe armor"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Ярость: броня увеличивается\nобратно пропорционально здоровью"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Rage: Plus il est endommagé,\nplus sa cuirasse est solide"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Je mehr Schaden genommen wird,\ndesto stärker ist die Rüstung"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"暴怒-伤害越大，盔甲就越坚固"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Can produce more larves"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Плодит новых личинок"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Donne naîssance aux petit larves"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Kann neue Larven gebären"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"能够产生更多幼体"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Телепорт"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleportieren"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"传送"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Force field generator increase the armor\nof the enemies nearby"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Генератор силового поля увеличивает\nброню окружающих"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Générateur de champ de forces améliore les\ncuirasses des ennemis à proximité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Kraftfeldgenerator erhöht die Rüstung\nder Feinde in der Nähe"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"力场发生器增加了附近敌人的护甲"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Universal repair kit restores health to every\nenemy nearby, but not itself."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Ремонтное оборудование позволяет\nвосстанавливать некоторое количество здоровья"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Kit de réparation sert à redonner des points de\nvie aux ennemis à proximité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Das universelle Reparaturset stellt die Gesundheit\njedes Feindes in der Nähe wieder her,\naber nicht sich selbst."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"通用修理工具能恢复附近所有敌人的生命，但不能恢复自身的生命。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Shoots a nanobot web,\ntemporarily disabling the tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Может выпускать паутину из наноботов,\nкоторая временно выводит из строя башню"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Il tire des toiles d'araignée en nanites\nafin de neutraliser temporairement une tour"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Schießt ein Web ab und deaktiviert\nden Turm vorübergehend"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"发射一个纳米机器人网络，暂时关闭信号塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Has an energy shield that gets\nwicker every second."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Оснащен энергетическим щитом,\nвеличина заряда которого уменьшается со временем"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Son bouclier d'énergie devient\nprogressivement de plus en plus faible"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Der Energieschild wird mit jeder\nSekunde stärker."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"有一个能量盾，每秒钟都会变弱。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Its equipment is able to cause a temporary\nbreakage of Tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Оснащен излучателем направленного действия,\nкоторый может вызывать поломки башен"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Son équipement sert à mettre les\ntours en panne temporaire"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Es wird deinen Turm zerstören."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"它的设备能够暂时性破坏塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Enhanced Regeneration restores\na Health of its carrier."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Усиленная регенерация\nпозволяет восстанавливать здоровье"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Se redonne les points de vie grâce\nà sa régénération "));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Verbesserte Regeneration stellt die\nGesundheit des Trägers wieder her"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"增强再生恢复其载体。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"It speeds up directly proportional to the\ndamage it took"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Ускоряется пропорционально отнятому здоровью"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Plus il est endommagé, plus rapide il se déplace"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Es beschleunigt direkt proportional zum\nerlittenen Schaden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"它的速度与它所受到的伤害成正比"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"It equipped with blasters able to destroy towers\nwith one shot"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Вооружен бластером сингулярности,\nимеющим тяжелые снаряды. Наиболее опасное\nоружие врага, может уничтожить башню\nс одного попадания."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Ses lasers peuvent détruire complètement une tour"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Es ist mit Strahlern ausgestattet,\ndie Türme mit einem Schuss zerstören können"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"它装备了爆能枪，一发就能摧毁塔楼"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"It equipped with guided missiles which\ndowngrade towers"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Вооружен самонаводящимися ракетами, при\nпопадании которых происходит\nухудшение башни."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Il est equipé des missiles dont l'explosion provoque\nun downgrade d'une tour "));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Es ist mit Lenkflugkörpern ausgestattet,\ndie Türme herabstufen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"它装备了导弹，可以降低塔的等级"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"It can get faster by curling up and rolling"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"Может сворачиваться в клубок и катиться,\nчто значительно увеличивает его скорость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"Lové, il peut se déplacer en roulant ce qui le\nrend plus rapide"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"Es kann schneller werden, indem es sich zusammenrollt und rollt"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"卷曲和滚动可以加快速度"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"It moves around by hopping "));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"Передвигается прыжками"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"Il se déplace en sautant"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"Es bewegt sich durch Hüpfen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"它跳来跳去"));

	//panel
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Acid"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Кислота"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Acide"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Säure"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"酸"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bomb"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Бомба"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"炸弹"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Bombe-glace"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Frostbombe"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"冷冻炸弹"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Увеличить скорострельность"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour à vitesse\nd'attaque augmentée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Erhöhen Sie\ndie Feuerrate"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"加速塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Усиление башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages\naugmentés"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Verursachter\nSchaden steigt"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"造成的伤害增加"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stasis"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Стазис"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stase"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stasis"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"停滞"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Base tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Обычная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Tour de base"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Gewöhnlicher Turm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"基塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Energy tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Энергетическая башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Tour de production\nd'énergie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Energieturm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"能量塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Rocket tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Ракетная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Tour reacteur-fusée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Raketenturm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"火箭塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Tower freeze"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Замораживающая башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Tour a obus glaçants"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Gefrierturm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"冷冻塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Tower laser"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Лазерная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Tour laser"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Laserturm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"镭射塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Tower improved"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Улучшенная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Tour de base améliorée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Verbesserter Turm"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"改造塔"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Level"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Уровень"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Niveau"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Level"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"等级"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Damage per seconds"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Урон в секунду"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Dégâts par seconde"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Schaden pro Sekunde"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"每秒损害"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"DPS"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"Урон в сек"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"DPS"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"DPS"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"每秒损害"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Radius"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Радиус"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Rayon"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Radius"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"镭射"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Destroyed targets"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Целей уничтожено"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Ennemis tués"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Morde"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"已被攻陷目标"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Cost"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Цена"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Prix"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Preis"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"成本"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Sell cost"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Цена продажи"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Prix de la vente"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Verkaufspreis"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"售价"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Wave"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Волна"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Vague"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Welle"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"浪"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limit"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Лимит"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limit"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"限制"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Has a cumulative effect\nof reducing enemy armor"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Effet cumulatif de reduire\nla cuirasse d'un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Hat einen kumulativen Effekt\nder Reduzierung der feindlichen Rüstung"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"会逐渐减少敌人的装甲"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Verlangsamt den Feind"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"减慢敌人的速度"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Produces resources"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Производит ресурсы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Extraire les ressourses"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Produziert Ressourcen"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"生产资源"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон по площади"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Dommage d'une zone"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Raketenexplosion verursacht Flächenschaden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"火箭弹爆炸造成区域伤害"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Kontinuierlicher Angriff"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"持续攻击"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем показателям"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"In jeder Hinsicht am effektivsten"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"所有统计数据中最有效的"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Exploision instantanée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Verursacht sofort Schaden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"立刻造成伤害"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantané d'un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Verlangsamt sofort Feinde"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"立刻减慢敌人的速度"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Непрерывно наносит урон в области"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Dommages prolongés d'une certaine territoire"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Verursacht kontinuierlich Flächenschaden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"连续不断地造成区域性伤害"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Amélioration temporaire des dommages causés"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Erhöht vorübergehend den Turmschaden"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"暂时增加塔楼损坏"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Accélération temporaire des tirs"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Erhöht vorübergehend die Angriffsgeschwindigkeit\ndes Turms"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"暂时增加塔的攻击速度"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Останавливает движение\nвсех врагов"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stoppt die Bewegung aller Feinde"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"阻止所有敌人的活动"));

	//map effects
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Mist"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Туман"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Brouillard"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Nebel"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"薄雾"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Energy leech"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Энергопиявка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Sangsue d'énergie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Energie Blutegel"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"能量吸取器"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Bushes"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Заросли"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Broussailles"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Dickicht"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"灌木"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"ICBM"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"МБР"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"Missile balistique\nintercontinental"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"Interkontinentalrakete"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"洲际弹道导弹"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"The Goo"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"Жижа"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"la Substance"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"Sabsche"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"粘性物质"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Blind spot"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Слепая зона"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Angle mort"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Blinder Fleck"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"盲点"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"Being hit in the tower will limit\nthe accuracy of the guidance devices.\nReduces turret firing range."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"При попадании на башню ограничивет\nточность приборов наведения.\nУменьшает радиус стрельбы башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"Une tour entourée du brouillard a le rayon\nd'action diminué"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"Wenn Sie im Turm getroffen werden, wird die\nGenauigkeit der Führungsgeräte eingeschränkt.\nReduziert die Schussreichweite des Turmes."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"被击中会限制制导装置的精确度，减少炮塔射程。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Sucks out the energy from the energy towers.\nMay be destroyed by a bomb explosion"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Выкачивает энергию из энергетических башен.\nМожет быть уничтожен взрывом бомбы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Elle suce l'énérgie des tours.\nL'explosion d'une bombe peut la faire fuir."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Saugt die Energie aus den Energietürmen heraus.\nKann durch eine Bombenexplosion zerstört werden."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"从能量塔中吸取能量-可能会被炸弹爆炸摧毁"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Interferes with the tower operations.\nDamage and speed are reduced."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Вызывает помехи в работе башен,\nснижается урон и скорость стрельбы."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Les broussailles empèchent aux tours de\nfonctionner correctement, elles affectent les\ndommages causés est la vitesse du tir"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Eingriffe in den Turmbetrieb.\nSchaden und Geschwindigkeit werden reduziert."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"会干扰塔台的运作，损害和速度都会降低。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"It simply destroys the tower."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Уничтожает башню независимо от уровня."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Il peut détruir complètement une tour"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Es zerstört einfach den Turm."));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"它只会摧毁塔楼。"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"A very mysterious substance can sometimes come\nout of the ground. It doesn't harm previously built\nTowers but it is impossible to construct new\nTowers on it. Hopefully, this anomaly doesn't\nlast long"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"В некоторых местах из планетарной коры может\nвыступать некая субстанция, на которой невозможно\nпостроить новую башню. Благодаря нашей\nпродвинутой технологии установки фундамента на\nуже построенные башни аномалия не влияет.\nИмеет временный эффект и через\nопределенное время исчезает."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"La Substance mystériouse remonte parfois à la\nsurface, où elle empèche de construire de\nnouvelles tours. Les tours déjà construites ne sont\npas normalement endomagées par la Substance,\nen plus, son activité ne dire pas longtemps."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"Manchmal kann eine sehr mysteriöse Substanz aus\ndem Boden kommen. Es schadet zuvor gebauten\nTürmen nicht, aber es ist unmöglich, neue Türme\ndarauf zu bauen. Hoffentlich hält diese Anomalie\nnicht lange an"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"一种非常神秘的物质有时会从地下冒出来。\n它不会伤害以前建造的塔，但是不可能在上面建造新的塔。\n希望这个异常点不会持续太久"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Such abnormal zones appear and disappear\nrandomly, they hide enemies lurking underneath"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Подобные аномальные зоны скрывают от наших\nрадаров врагов, попадающих в область их\nраспространения. Наши технологии против них\nбессильны. К счастью, эти аномалии недолговечны\nи исчезают так же внезапно, как и появляются."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Ces zones d'anomalies empèchent à nos tours de\ndetecter des ennemis traversant ces secteurs.\nHeureusement, elles disparaissent aussi vite\nqu'elles apparaissent. "));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Solche abnormalen Zonen erscheinen und\nverschwinden zufällig, sie verstecken darunter\nlauernde Feinde"));
	chineseTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"这些异常区域随机出现和消失，它们隐藏了潜伏在下面的敌人"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::SHADERS, L"Shaders"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SHADERS, L"Шейдеры"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SHADERS, L"Shaders"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::SHADERS, L"???"));
	chineseTranslation.insert(TR_PAIR(TranslationsManager::SHADERS, L"???"));

	Engine::Instance().translationsManager()->addTranslation(std::string("russian"), russainTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("english"), englishTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("french"), frenchTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("german"), germanTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("schinese"), chineseTranslation);
}
