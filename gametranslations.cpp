#include "gamemanagers.h"
#include "engine.h"

#define TR_PAIR(X,Y) std::pair<int, sf::String>(X, Y)

void GameManagers::loadTranslations()
{
	std::map<int, sf::String> russainTranslation;
	std::map<int, sf::String> englishTranslation;
	std::map<int, sf::String> frenchTranslation;
	std::map<int, sf::String> germanTranslation;

	russainTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"Русский"));
	englishTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"French"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE_ID, L"Deutsch"));

	//menu
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Play"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Играть"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Jouer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::START, L"Spiel"));

	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Опции"));
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Options"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::OPTIONS, L"Optionen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Справочник"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::MANUAL, L"Manual"));

	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Выход"));
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Exit"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Sortir du jeu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT, L"Ende"));

	//manual
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Speed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Скорость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Vitesse"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SPEED, L"Drehzahl"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Size"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Размер"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Taille"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SIZE, L"Abmessung"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Armour"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Броня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Blindage"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR, L"Panzer"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Health points"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Points de vie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEALTH, L"Gesundheit"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Abilities"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Pouvoirs"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY, L"Fähigkeiten"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Small"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Мелкий"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Petite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMALL, L"Klein"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Mid"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Средний"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Moyenne"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::MID, L"Mittelfein"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Big"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Крупный"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Grande"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BIG, L"Große"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Slow"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Медленная"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Petite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLOW, L"Langsam"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normale"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Fast"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Быстрая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Grande"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FAST, L"Schnell"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Next"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Вперед"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Suivant"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::NEXT, L"Nachfolgend"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Previous"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Précédent"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::PREVIOUS, L"Vorausgehend"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Credits"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Авторы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Auteurs"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CREDITS, L"Autoren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Back"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Вернуться"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Revenir"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BACK, L"Zurück"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Light"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Легкая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Léger"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_LIGHT, L"Leicht"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Medium"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Средняя"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Moyen"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_MEDIUM, L"Mittlere"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Strong"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Крепкая"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Épais"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ARMOR_STRONG, L"Schwere"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Cooldown time"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Время перезарядки"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Temps de rechargement"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::COOLDOWN_TIME, L"Abfahren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Урон"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Damage"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_ATTRIBUTE, L"Schaden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Время действия"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Duration"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DURATION_ATTRIBUTE, L"Zeitdauer"));

	//settings
	englishTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Settings"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Настройки"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Parametres"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::SETTINGS, L"Einstellung"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Sound"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Звуки"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Sons"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::SOUND, L"Schall"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Music"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Музыка"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Musique"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::MUSIC, L"Musik"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Fullscreen"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Полноэкранный режим"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Plein ecran"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::FULLSCREEN, L"Vollbildschirm"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Resolution"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Разрешение"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Resolution"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::RESOLUTION, L"Auflösung"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Language"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Язык"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Langue"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::LANGUAGE, L"Sprache"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Accept"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Принять"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Confirmer"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::ACCEPT, L"Annehmen"));

	englishTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Cancel"));
	russainTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Отмена"));
	frenchTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Supprimer"));
	germanTranslation.insert(TR_PAIR(TranslationsManager::CANCEL, L"Abbrechen"));

	//game
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Paused"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Пауза"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Pause"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::PAUSED, L"Pause"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Continue"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Продолжить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Continuer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONTINUE, L"Weiterführen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Restart"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Рестарт"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Recommencer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::RESTART, L"Neustart"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Exit to menu"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Выход в меню"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Sortir au menu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_TO_MENU, L"Zum Menü\nzurückkehren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Exit from game"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Выход из игры"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Sortir du jeu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::EXIT_FROM_GAME, L"Beende das Spiel"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"You lose!\nTry again"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Вы проиграли!\nПопробуйте еще"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Désolé, t'as pérdu, on recommense?"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::GAME_OVER, L"Du verlierst!\nVersuch es noch einmal"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations!\nLevel completed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Поздравляем!\nУровень пройден"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Cool! T'as réussi!"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CONGRATULATIONS, L"Herzliche Glückwünsche!\nLevel abgeschlossen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Press Space to start"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Нажмите Пробел чтобы начать"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Tapez l'Espace afin de commencer le jeu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::START_GAME, L"Drücken Sie die Leertaste, um zu starten"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Press Space to skip"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Нажмите Пробел чтобы пропустить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Tapez l'Espace pour ignorer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SKIP_TEXT, L"Drücken Sie die Leertaste, um zu überspringen"));

	//instructions
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Hello there! Welcome to Arc Defence game.\nWe really need your help here.\nI’ll get you acquaintance with details."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Добро пожаловать в игру Arc Defence.\nНам нужна твоя помощь в защите территории.\nЯ введу тебя в курс дела."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Bonjour, mon pote! T’es maintenant au monde d’Arc Defence.\nNous avons vraiment besoin de ton aide.\nJe vais te renseigner comment on se débrouille là."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Hallo und willkommen bei Arc Defense.\nWir brauchen wirklich Ihre Hilfe.\nIch werde Sie mit Details vertraut machen."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Energy\nEnergy is crucial for us.\nWe can extract a big amount of his resource due to\nEnergy tower. It is also possible to gain a bit of energy\nby exterminating enemies."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Энергия\nЭто наш основной ресурс.\nВ большом количестве ее можно добыть благодаря\nЭнергетической башне. Уничтожение врага тоже немного\nпополнит запас энергии."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Énergie\nElle est essentielle pour nous.\nOn en extrait en grande quantité grâce aux Tours de\nproduction d'énergie. On peut également en obtenir en peu\naprès avoir tuer un ennemi."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Energie.\nEs ist unsere entscheidende Ressource.\nEin großer Teil davon kann über einen Energieturm\ngewonnen werden. Es ist auch möglich,\nein bisschen Energie zu gewinnen, indem man die\nFeinde ausrottet."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health scale\nWe can’t let them go further! Only a little amount of enemies\ncan cross the line without endanger our defensive campaign.\nEvery single enemy crossing the finish line takes up some\nhealth points from you: the bigger enemy is,\nthe more points are taken."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Шкала жизни\nМы не можем допустить, чтобы большое количество врагов\nперешло рубеж. Каждый враг, которому удается от нас уйти,\nуменьшает показатели шкалы на определенное количество очков.\nОбрати внимание, что количество очков зависит от того,\nнасколько они опасны: небольшие не причиняют большого вреда,\nа мощным необходимо оказать решительный отпор."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Points de vie\nOn ne doit absolument pas laisser les ennemis passer plus loin.\nL’échelle des points de vie indique la quantité des ennemis qui\npeuvent passer la frontière sans représenter l’ultime menace.\nS’il ne reste plus de points de vie, on est foutus.\nDes ennemis géants sont plus dangereux!"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Gesundheitsskala\nWir können sie nicht weiter gehen lassen! Nur wenige Feinde\n können die Grenze überschreiten, ohne unsere\nVerteidigungskampagne zu gefährden. Jeder einzelne Feind,\nder die Ziellinie überquert, nimmt einige Gesundheitspunkte\nvon Ihnen auf: Je größer der Feind ist, desto mehr\nPunkte werden vergeben."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress\nOur enemies tend to hit in waves, fortunately, we can predict\napproximate number of waves, you’ll see it here."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Прогресс уровня.\nОбычно наши враги нападают волнами. Мы можем предсказать их\nпримерное количество и вывести тебе на шкалу."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Avancement\nAfin d’attaquer les ennemis forment une sorte des vagues.\nNous pouvons prévoir le nombre des vagues, cette échelle\nsert à l’indiquer pour toi."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Fortschritt\nUnsere Feinde neigen dazu, in Wellen zu schlagen.\nGlücklicherweise können wir die ungefähre Anzahl der Wellen\nvorhersagen. Sie werden sie hier sehen."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Towers\nThere are two types of Towers: the Attacking towers and\nthe Energy one. You should build the Attacking towers in\nthe marked zones near the Energy ones to provide them\nnecessary resources. Upgrading an Energy tower helps\nto expand this zone."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Башни\nБашни можно разделить на Ресурсные и Атакующие.\nИз доступных нам башен только Энергетическая относится\nк первому виду. Атакующие башни необходимо строить в\nотмеченной зоне вокруг Энергетической, что позволит ей\nснабжать их необходимой энергией. При повышении уровня\nЭнергетической башни эта зона увеличивается."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Tours\nIl existe deux types des Tours: celles de\nproduction et celles d'attaque. Il est impossible de construir des\nTours d'attaque loin des Tours de production d'énergie.\nIl faut les placer dans la zone indiquée autour des Tours de\nproduction, ce qui les permettera de proqurer les Tours\nd'attaque d'énergie nécéssaire. La zone grandisse avec chaque\nupgrade de la Tour de production"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Türme\nDort die Angriffstürme und der Energieturm.\nSie sollten die Angriffstürme in markierten Zonen in der Nähe\nderEnergietürme errichten, um die erforderlichen Ressourcen\nbereitzustellen. Durch die Aufrüstung eines\nEnergieturms wird diese Zone erweitert."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Tower upgrade and tower selling\nYou can improve towers characteristics by upgrading them.\nIf you ran out of energy and can't afford any upgrade,\nyou can wait until your Energy towers extract the amount\nyou need or you can sell one of your previously builded Towers.\nTo sell or upgrade a Tower you need to click on it and choose\nnecessary option."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Улучшение и продажа башен\nМожно улучшать качественные характеристики башен путем их\nапгрейдаза определенное количество энергии. Если не хватает\nэнергии, подожди, пока Ресурсной башне удастся добыть нужный\nобъем, или продай башню, которая плохо справляется со своей\nзадачей. Для улучшения и продажи нажми на башню и выбери\nнеобходимый пункт."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Upgrade et vente des Tours \n Tu peux améliorer les charactéristiques des Tours\ngrâce à l'upgrade. Si tu n'as pas d'énergie pour payer, tu peux\nattendre lorsque la Tour de production en extrait assez ou\nvendre une autre Tour. Pour effectuer l'upgrade ou la vente\ntu a à taper sur la Tour et choisir l'option nécéssaire."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_UPGRADE_SELL, L"Turm-Upgrade und Turmverkauf\nSie können die Eigenschaften von Türmen verbessern,\nindem Sie darauf klicken. Wenn Ihnen die Energie ausgeht und\nSie sich kein Upgrade leisten können, können Sie warten,\nbis Ihre Energietürme die benötigte Menge extrahiert haben,\noder Sie können einen Ihrer zuvor gebauten Türme verkaufen."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"So it’s time for you to show us what you can do!\nGood luck!"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"А теперь покажи нам, на что способен!\nУдачи!"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"Bon, maintenant c’est à toi d’agir!\nBonne chance!"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_GOOD_LUCK, L"Es ist Zeit für Sie, uns zu zeigen, wozu Sie fähig sind!\nViel Glück!"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Нажмите Ввод, чтобы продолжить, или Пробел, чтобы пропустить"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Taper la touche Entree pour continuer ou l'Espace pour ignorer"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Drücken Sie die Eingabetaste, um fortzufahren, oder die Leertaste, um zu überspringen"));

	//enemies
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infantry squad"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Отряд пехоты"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infanterie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INFANTRY, L"Infanterietrupp"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"War vehicle\n«Praefectus»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Боевая машина\n«Префект»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Véhicule de combat\n«Préfet»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAR_VEHICLE, L"Kampffahrzeug\n«Präfekt»"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Recon tricycle «IS2»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Разведывательный\nтрицикл «ИС2»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Tricycle de\nreconnaissance «IS2»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRICYCLE, L"Aufklärungsdreirad IS2"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Tank «W-100»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Танк «В-100»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Char «W-100»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TANK, L"Tank «W-100»"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Mechspider"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Мехпаук"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"MechAraignée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::MECHSPIDER, L"Mechspinne"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Туатара"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TUATARA, L"Tuatara"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Giant slug"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Гигантский слизняк"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Limace géante"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::GIANT_SLUG, L"Riesenschnecke"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Heavy tank\n«Juggernaut»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Тяжелый танк\n«Джаггернаут»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Char lourd\n«Juggernaut»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::HEAVY_TANK, L"Schwerer Panzer\n«Juggernaut»"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Sluggy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Слизнячок"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Petit limace"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SLUGGY, L"Schnecklein"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"ARV «Immunes»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"БРЭМ «Иммун»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"ARV «Immune»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::REPAIR, L"Bergepanzer «Immun»"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Defense combat\nvehicle «Tachyon»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Оборонительная боевая\nмашина «Тахион»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Véhicule à écran de\ndéfence «Tachyon»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SHELL, L"Defensives\nKampffahrzeug «Tachyon»"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Walker «33-Qc. rev02.9»"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Шагоход «33-Кц. рев02.9»"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Marcheur «33-Qc. rev02.9»"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WALKER, L"Wanderer «33-Qc. rev02.9»"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Scarbreed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Шраморожденный"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Balafre-né"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROLLER, L"Narbengeboren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"SPG-4000"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"САУ-4000"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"Automoteur\nd'artillerie-4000"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TRACTOR, L"Selbstfahrlafette-4000"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larva"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Личинка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larve"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WORM, L"Larve"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Bugsaurus"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Жукозавр"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Insectosaure"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BUGSAURUS, L"Käfersaurier"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Ammonite"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Аммонит"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Ammonite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELFHEAL, L"Ammoniten"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Acrididae"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Акрид"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Acrididae"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::JUMPER, L"Acrididae"));

	//towers abilities
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnerability"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Неуязвимость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Invulnérabilité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_ABILITY, L"Unverwundbarkeit"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Area freeze"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Заморозка по площади"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Congélation d'une zone"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_ABILITY, L"Bereich einfrieren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Periodic energy impulses,\ndamaging nearby enemies"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Периодично испускает энергетические импульсы,\nнаносящие урон врагам рядом"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Impultions d'énergie périodiques endommageant\ndes ennemis à proximité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_ABILITY, L"Periodische Energieimpulse,\ndie in der Nähe befindliche Feinde beschädigen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Burn enemy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Поджиг врага"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Brûle un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_ABILITY, L"Verbrenne den Feind"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Несколько целей"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Multi-targeting"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_ABILITY, L"Mehrere Feinde anvisieren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"4-way shooting"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Стрельба на 4 стороны"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"Shoot on 4 sides"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_ABILITY, L"4-Wege-Aufnahme"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Building cost increases with every new tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Цена постройки увеличивается\nс каждой построенное башней"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Construction d'une chaque nouvelle\ntour augmente le prix de la suivante"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::POWER_TOWER_TRAIT, L"Die Baukosten steigen mit jedem neuen Turm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Can build anywhere"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Можно построить в любом месте"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Peut être construite en tout lieu"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_ANYWHERE, L"Kann überall gebaut werden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Can build only near energy tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Можно построить только возле\nЭнергетической башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Ne peut être construite qu'à proximité\nd'une Tour de production d'énergie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::CAN_BUILD_NEAR_ENERGY_TOWER, L"Kann nur in der Nähe des Energieturms\ngebaut werden"));

	//enemies abilities
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"None"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Отсутствует"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Absent"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_NONE, L"Keiner"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Rage: The more damage taken - the stronger\nthe armor"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Ярость: броня увеличивается\nобратно пропорционально здоровью"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Rage: Plus il est endommagé,\nplus sa cuirasse est solide"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_RAGE, L"Je mehr Schaden genommen wird,\ndesto stärker ist die Rüstung"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Can produce more larves"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Плодит новых личинок"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Donne naîssance aux petit larves"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SPAWN, L"Kann neue Larven gebären"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Телепорт"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleport"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_TELEPORT, L"Teleportieren"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Force field generator increase the armor\nof the enemies nearby"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Генератор силового поля увеличивает\nброню окружающих"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Générateur de champ de forces améliore les\ncuirasses des ennemis à proximité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHELL_NEAR, L"Kraftfeldgenerator erhöht die Rüstung\nder Feinde in der Nähe"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Universal repair kit restores health to every\nenemy nearby, but not itself."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Ремонтное оборудование позволяет восстанавливать\n некотороеколичество здоровья"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Kit de réparation sert à redonner des points de\nvie aux ennemis à proximité"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_HEAL_NEAR, L"Das universelle Reparaturset stellt die Gesundheit\njedes Feindes in der Nähe wieder her,\naber nicht sich selbst."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Shoots a nanobot web,\ntemporarily disabling the tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Может выпускать паутину из наноботов,\nкоторая временно выводит из строя башню"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Il tire des toiles d'araignée en nanites\nafin de neutraliser temporairement une tour"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SHUTDOWN_TOWER, L"Schießt ein Web ab und deaktiviert\nden Turm vorübergehend"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Has an energy shield that gets\nwicker every second."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Оснащен энергетическим щитом,\nвеличина заряда которого уменьшается со временем"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Son bouclier d'énergie devient\nprogressivement de plus en plus faible"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_STRONG, L"Der Energieschild wird mit jeder\nSekunde stärker."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Its equipment is able to cause a temporary\nbreakage of Tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Оснащен излучателем направленного действия,\nкоторый может вызывать поломки башен"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Son équipement sert à mettre les\ntours en panne temporaire"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWN_TOWER, L"Es wird deinen Turm zerstören."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Enhanced Regeneration restores\na Health of its carrier."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Усиленная регенерация\nпозволяет восстанавливать здоровье"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Se redonne les points de vie grâce\nà sa régénération "));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_SELF_HEAL, L"Verbesserte Regeneration stellt die\nGesundheit des Trägers wieder her"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"It speeds up directly proportional to the damage it took"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Ускоряется пропорционально отнятому здоровью"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Plus il est endommagé, plus rapide il se déplace"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_FASTER, L"Es beschleunigt direkt proportional zum erlittenen Schaden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"It equipped with blasters able to destroy towers with one shot"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Вооружен бластером сингулярности, имеющим тяжелые снаряды. Наиболее опасное оружие врага, может уничтожить башню с одного попадания."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Ses lasers peuvent détruire complètement une tour"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_KILL_TOWER, L"Es ist mit Strahlern ausgestattet, die Türme mit einem Schuss zerstören können"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"It equipped with guided missiles which downgrade towers"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Вооружен самонаводящимися ракетами, при попадании которых происходит ухудшение башни."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Il est equipé des missiles dont l'explosion provoque un downgrade d'une tour "));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_DOWNGRADE_TOWER, L"Es ist mit Lenkflugkörpern ausgestattet, die Türme herabstufen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"It can get faster by curling up and rolling"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"Может сворачиваться в клубок и катиться, что значительно увеличивает его скорость"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"Lové, il peut se déplacer en roulant ce qui le rend plus rapide"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_ROLLING, L"Es kann schneller werden, indem es sich zusammenrollt und rollt"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"It moves around by hopping "));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"Передвигается прыжками"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"Il se déplace en sautant"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENEMY_ABILITY_JUMPING, L"Es bewegt sich durch Hüpfen"));

	//panel
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Acid"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Кислота"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Acide"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_ACID, L"Säure"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bomb"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Бомба"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Bombe-glace"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Frostbombe"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Увеличить скорострельность"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour à vitesse\nd'attaque augmentée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Erhöhen Sie\ndie Feuerrate"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Усиление башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages\naugmentés"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Verursachter\nSchaden steigt"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stasis"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Стазис"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stase"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ABILITY_STOP, L"Stasis"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Base tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Обычная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Tour de base"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_BASE, L"Gewöhnlicher Turm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Energy tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Энергетическая башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Tour de production\nd'énergie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_POWER, L"Energieturm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Rocket tower"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Ракетная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Tour reacteur-fusée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_ROCKET, L"Raketenturm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Tower freeze"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Замораживающая башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Tour a obus glaçants"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_FREEZE, L"Gefrierturm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Tower laser"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Лазерная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Tour laser"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_LASER, L"Laserturm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Tower improved"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Улучшенная башня"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Tour de base améliorée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_IMPROVED, L"Verbesserter Turm"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Level"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Уровень"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Niveau"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LEVEL, L"Level"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Damage per seconds"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Урон в секунду"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Dégâts par seconde"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Schaden pro Sekunde"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"DPS"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"Урон в сек"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"DPS"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DPS, L"DPS"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Radius"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Радиус"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Rayon"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::RADIUS, L"Radius"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Destroyed targets"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Целей уничтожено"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Ennemis tués"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::KILLS, L"Morde"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Cost"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Цена"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Prix"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::COST, L"Preis"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Sell cost"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Цена продажи"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Prix de la vente"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SELL_COST, L"Verkaufspreis"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Wave"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Волна"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Vague"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::WAVE, L"Welle"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limit"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Лимит"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limite"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LIMIT, L"Limit"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Has a cumulative effect\nof reducing enemy armor"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Effet cumulatif de reduire\nla cuirasse d'un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Hat einen kumulativen Effekt\nder Reduzierung der feindlichen Rüstung"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Verlangsamt den Feind"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Produces resources"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Производит ресурсы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Extraire les ressourses"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ENERGY_TOWER_DESCRIPTION, L"Produziert Ressourcen"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон по площади"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Dommage d'une certaine territoire"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Raketenexplosion verursacht Flächenschaden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Kontinuierlicher Angriff"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем показателям"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"In jeder Hinsicht am effektivsten"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Exploision instantanée"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Verursacht sofort Schaden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantané d'un ennemi"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Verlangsamt sofort Feinde"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Непрерывно наносит урон в области"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Dommages prolongés d'une certaine territoire"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::ACID_ABILITY_DESCRIPTION, L"Verursacht kontinuierlich Flächenschaden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Amélioration temporaire des dommages causés"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Erhöht vorübergehend den Turmschaden"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Accélération temporaire des tirs"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Erhöht vorübergehend die Angriffsgeschwindigkeit des Turms"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Останавливает движение\nвсех врагов"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stoppt die Bewegung aller Feinde"));

	//map effects
	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Mist"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Туман"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Brouillard"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_NAME, L"Nebel"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Energy leech"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Энергопиявка"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Sangsue d'énergie"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_NAME, L"Energie Blutegel"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Bushes"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Заросли"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Broussailles"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_NAME, L"Dickicht"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"ICBM"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"МБР"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"Missile balistique\nintercontinental"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_NAME, L"Interkontinentalrakete"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"The Goo"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"Жижа"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"la Substance"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_NAME, L"Sabsche"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Blind spot"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Слепая зона"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Angle mort"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_NAME, L"Blinder Fleck"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"Being hit in the tower will limit\nthe accuracy of the guidance devices.\nReduces turret firing range."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"При попадании на башню ограничивет\nточность приборов наведения.\nУменьшает радиус стрельбы башни"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"Une tour entourée du brouillard a le rayon d'action diminué"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::SMOKE_DESCRIPTION, L"Wenn Sie im Turm getroffen werden, wird die\nGenauigkeit der Führungsgeräte eingeschränkt.\nReduziert die Schussreichweite des Turmes."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Sucks out the energy from the energy towers.\nMay be destroyed by a bomb explosion"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Выкачивает энергию из энергетических башен.\nМожет быть уничтожен взрывом бомбы"));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Elle suce l'énérgie des tours.  L'explosion d'une bombe peut la faire fuir."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::DRAIN_DESCRIPTION, L"Saugt die Energie aus den Energietürmen heraus.\nKann durch eine Bombenexplosion zerstört werden."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Interferes with the tower operations.\nDamage and speed are reduced."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Вызывает помехи в работе башен,\nснижается урон и скорость стрельбы."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Les broussailles empèchent aux tours de fonctionner correctement, elles affectent les dommages causés est la vitesse du tir"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::REGRESS_DESCRIPTION, L"Eingriffe in den Turmbetrieb.\nSchaden und Geschwindigkeit werden reduziert."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"It simply destroys the tower."));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Уничтожает башню независимо от уровня."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Il peut détruir complètement une tour"));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::TOWER_EXPLOSION_DESCRIPTION, L"Es zerstört einfach den Turm."));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"A very mysterious substance can sometimes come out of the ground. It doesn't harm previously built Towers but it is impossible to construct new Towers on it. Hopefully, this anomaly doesn't last long"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"В некоторых местах из планетарной коры может выступать некая субстанция, на которой невозможно построить новую башню. Благодаря нашей продвинутой технологии установки фундамента на уже построенные башни аномалия не влияет. Имеет временный эффект и через определенное время исчезает."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"La Substance mystériouse remonte parfois à la surface, où elle empèche de construire de nouvelles tours. Les tours déjà construites ne sont pas normalement endomagées par la Substance, en plus, son activité ne dire pas longtemps."));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::LAVA_DESCRIPTION, L"Manchmal kann eine sehr mysteriöse Substanz aus dem Boden kommen. Es schadet zuvor gebauten Türmen nicht, aber es ist unmöglich, neue Türme darauf zu bauen. Hoffentlich hält diese Anomalie nicht lange an"));

	englishTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Such abnormal zones appear and disappear randomly, they hide enemies lurking underneath"));
	russainTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Подобные аномальные зоны скрывают от наших радаров врагов, попадающих в область их распространения. Наши технологии против них бессильны. К счастью, эти аномалии недолговечны и исчезают так же внезапно, как и появляются."));
	frenchTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Ces zones d'anomalies empèchent à nos tours de detecter des ennemis traversant ces secteurs. Heureusement, elles disparaissent aussi vite qu'elles apparaissent. "));
	germanTranslation.insert(TR_PAIR(GAME_TRANSLATION::INVISIBILITY_DESCRIPTION, L"Solche abnormalen Zonen erscheinen und verschwinden zufällig, sie verstecken darunter lauernde Feinde"));

	Engine::Instance().translationsManager()->addTranslation(std::string("russian"), russainTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("english"), englishTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("french"), frenchTranslation);
	Engine::Instance().translationsManager()->addTranslation(std::string("german"), germanTranslation);
}
