#include "gamemanagers.h"
#include "engine.h"
#include "managers.h"

GameManagers::GameManagers()
{

}

void GameManagers::loadTextures()
{
	Engine::Instance().texturesManager()->addTexture(TexturesManager::CLOSE_BACKGROUND, "images/close.png");
	Engine::Instance().texturesManager()->addTexture(TexturesManager::ABOUT_BACKGROUND, "images/about.png");
	Engine::Instance().texturesManager()->addTexture(TexturesManager::SETTINGS_BACKGROUND, "images/options.png");
	Engine::Instance().texturesManager()->addTexture(TexturesManager::INTRO_BACKGROUND, "images/logo.png");
	Engine::Instance().texturesManager()->addTexture(TexturesManager::FOCUS_ICON, "images/ui/focus.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::PANEL_TEXTURE, "images/ui/panel.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::CURSOR_TEXTURE, "images/ui/game_cursor.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::STAR_TEXTURE, "images/ui/star.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::EMPTY_STAR_TEXTURE, "images/ui/empty_star.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::SELL_TEXTURE, "images/ui/sell.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::UPGRADE_TEXTURE, "images/ui/upgrade.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::GAME_MENU_TEXTURE, "images/gameMenu.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::WINDOW_TEXTURE, "images/window.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::PANEL_CURSOR, "images/ui/panel_cursor.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MONEY_ICON, "images/ui/money.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::LIFE_ICON, "images/ui/money.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DIRECTION_TEXTURE, "images/ui/direction.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::LOCKED_ICON, "images/ui/locked.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::INSTRUCTIONS_TEXTURE, "images/ui/instructions.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::NEXT, "images/ui/right.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::PREVIOUS, "images/ui/left.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::EXIT, "images/ui/exit.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::CREDITS, "images/ui/credits.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_BOMB, "images/ui/Abilities/ability_bomb.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_FREEZE_BOMB, "images/ui/Abilities/ability_freeze_bomb.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_ACID, "images/ui/Abilities/ability_acid.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_DAMAGE, "images/ui/Abilities/ability_increase_tower_damage.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_INCREASE_TOWER_ATTACK_SPEED, "images/ui/Abilities/ability_increase_tower_speed.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ABILITY_TIME_STOP, "images/ui/Abilities/ability_stop.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_BASE, "images/ui/Towers/tower_base.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_POWER, "images/ui/Towers/tower_power.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_ROCKET, "images/ui/Towers/tower_rocket.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_FREEZE, "images/ui/Towers/tower_freeze.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_LASER, "images/ui/Towers/tower_laser.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::TOWER_IMPROVED, "images/ui/Towers/tower_improved.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BASE_PROJECTILE, "images/Weapons/bullet.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::IMPROVED_PROJECTILE, "images/Weapons/improved_bullet.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::FREEZE_PROJECTILE, "images/Weapons/freeze_bullet.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ROCKET_PROJECTILE, "images/Weapons/rocket.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::LASER_PROJECTILE, "images/Weapons/laser.png");

	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MENU_BACKGROUND, "images/menu.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MANUAL_BACKGROUND, "images/mission.png");

	//effects
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BASE_EXPLOSION_EFFECT, "images/Effects/base_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::FREEZE_EXPLOSION_EFFECT, "images/Effects/freeze_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ROCKET_EXPLOSION_EFFECT, "images/Effects/rocket_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::IMPROVED_EXPLOSION_EFFECT, "images/Effects/improved_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::VENOM_EFFECT, "images/Effects/venom.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BOMB_EXPLOSION, "images/Effects/bomb_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::FREEZE_BOMB_EXPLOSION, "images/Effects/freeze_bomb_explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::HEAL_EFFECT, "images/Effects/heal.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::SHELL_EFFECT, "images/Effects/shell.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::WEB, "images/Effects/web.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DOWN_EFFECT, "images/Effects/down.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DOWNGRADE_PROJECTILE, "images/Weapons/electric.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::UPGRADE, "images/Effects/upgrade.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::SMOKE, "images/Effects/smoke.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::EXPLOSION, "images/Effects/explosion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::DRAIN, "images/Effects/drain.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::REGRESS, "images/Effects/regress.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BURN, "images/Effects/burn.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::BLAST, "images/Effects/blast.png");

	//map icons
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_1, "images/maps_icons/mission1.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_2, "images/maps_icons/mission2.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_3, "images/maps_icons/mission3.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_4, "images/maps_icons/mission4.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_5, "images/maps_icons/mission5.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_6, "images/maps_icons/mission6.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_7, "images/maps_icons/mission7.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_8, "images/maps_icons/mission8.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_9, "images/maps_icons/mission9.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_10, "images/maps_icons/mission10.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_11, "images/maps_icons/mission11.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_12, "images/maps_icons/mission12.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_13, "images/maps_icons/mission13.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_14, "images/maps_icons/mission14.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::MAP_ICON_MISSION_15, "images/maps_icons/mission15.png");

	//enemies
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SCORPION, "images/Enemies/scorpion.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_CAR, "images/Enemies/car.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_TRICYCLE, "images/Enemies/tricycle.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_TANK, "images/Enemies/tank.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SPIDER, "images/Enemies/spider.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_HELICOPTER, "images/Enemies/helicopter.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_AIRCARRIER, "images/Enemies/aircarrier.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_BIG_TANK, "images/Enemies/big_tank.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_PLANE, "images/Enemies/plane.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_REPAIR, "images/Enemies/repair.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SHELL, "images/Enemies/shell.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_TELEPORT, "images/Enemies/teleport.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_SELF_HEAL, "images/Enemies/enemy.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_DOWN_TOWER, "images/Enemies/enemy1.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_ANT, "images/Enemies/ant.png");
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::ENEMY_COW, "images/Enemies/cow.png");

	//objects
	Engine::Instance().texturesManager()->addTexture(GAME_TEXTURE::OBJECT_TREE, "images/maps_objects/tree.png");
}

void GameManagers::loadSounds()
{
	Engine::Instance().soundManager()->addSound(GAME_SOUND::SELL, "sounds/sell.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::TARGET_LOCK, "sounds/target_lock.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::TOWER_EXPLOSION, "sounds/tower_explosion.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::SETUP, "sounds/setup.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::WARNING, "sounds/warning.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::WIN, "sounds/win.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::UPGRADE, "sounds/upgrade.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::INTRO, "sounds/intro.ogg");

	//enemies
	Engine::Instance().soundManager()->addSound(GAME_SOUND::CATCH, "sounds/enemies/catch.ogg");
	//abilities
	Engine::Instance().soundManager()->addSound(GAME_SOUND::BOMB, "sounds/abilities/bomb.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::STOP, "sounds/abilities/stop.ogg");
	//towers
	Engine::Instance().soundManager()->addSound(GAME_SOUND::FREEZE_SHOT, "sounds/towers/freeze_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::BASE_SHOT, "sounds/towers/base_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::ROCKET_SHOT, "sounds/towers/rocket_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::LASER_SHOT, "sounds/towers/laser_shot.ogg");
	Engine::Instance().soundManager()->addSound(GAME_SOUND::IMPROVED_SHOT, "sounds/towers/improved_shot.ogg");

}

void GameManagers::loadMusic()
{
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_1, "sounds/music/track_1.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_2, "sounds/music/track_2.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_3, "sounds/music/track_3.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_4, "sounds/music/track_4.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_5, "sounds/music/track_5.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_6, "sounds/music/track_6.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_7, "sounds/music/track_7.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::TRACK_8, "sounds/music/track_8.ogg");


	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_1, "sounds/music/final_1.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_2, "sounds/music/final_2.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::FINAL_3, "sounds/music/final_3.ogg");


	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::MENU, "sounds/main.ogg");
	Engine::Instance().soundManager()->addMusic(GAME_MUSIC::CREDITS, "sounds/credits.ogg");
}

void GameManagers::loadTranslations()
{
	map<int, wstring> russainTranslation;
	map<int, wstring> englishTranslation;
	map<int, wstring> frenchTranslation;


	russainTranslation.insert(pair<int, wstring>(TranslationsManager::LANGUAGE_ID, L"Русский"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::LANGUAGE_ID, L"English"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::LANGUAGE_ID, L"French"));

	//menu
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CAMPAIGN, L"Кампания"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CAMPAIGN, L"Play"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CAMPAIGN, L"Jouer"));

	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::OPTIONS, L"Опции"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::OPTIONS, L"Options"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::OPTIONS, L"Options"));

	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::MANUAL, L"Справочник"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT, L"Выход"));

	//settings
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::SETTINGS, L"Настройки"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::AUDIO, L"Аудио"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::SOUND, L"Звуки"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::MUSIC, L"Музыка"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::VIDEO, L"Видео"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::FULLSCREEN, L"Полноэкранный режим"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::RESOLUTION, L"Разрешение"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::MISC, L"Разное"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::LANGUAGE, L"Язык"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::ACCEPT, L"Принять"));
	russainTranslation.insert(pair<int, wstring>(TranslationsManager::CANCEL, L"Отмена"));

	//panel
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"Кислота"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"Бомба"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Морозная бомба"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Ускорение башни"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Увеличение урона башни"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_STOP, L"Остановка"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_BASE, L"Обычная башня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_POWER, L"Энергетическая башня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"Ракетная башня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"Замораживающая башня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_LASER, L"Лазерная башня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"Улучшенная башня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::LEVEL, L"Уровень"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Урон в секунду"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::RADIUS, L"Радиус"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::KILLS, L"Фраги"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::COST, L"Цена"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SELL_COST, L"Цена продажи"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::WAVE, L"Волна"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Имеет кумулятивный эффект\nпо снижению вражеской брони"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Замедляет врага"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"Производит ресурсы"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Взрыв от ракеты наносит урон\nпо площади"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Атакует непрерывно"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Наиболее эффективна по всем\nпоказателям"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Наносит урон мгновенно"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Замедляет врагов мгновенно"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"Непрерывно наносит урон\nв области"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Временно увеличивает урон\nбашни"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Временно увеличивает скорость\nатаки башни"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Останавливает движение всех\nврагов"));

	//game
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PAUSED, L"Пауза"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CONTINUE, L"Продолжить"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::RESTART, L"Рестарт"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"Выход в меню"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Выход из игры"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::GAME_OVER, L"Игра окончена"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"Поздравляем"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::START_GAME, L"Нажмите Пробел чтобы начать"));

	//instructions
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Добро пожаловать!"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Башни"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"Способности"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Валюта"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Здоровье"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Прогресс уровня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Нажмите Ввод чтобы продолжить или Пробел чтобы пропустить"));

	//enemies
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SCORPION, L"Скорпион"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CAR, L"Машина"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TANK, L"Танк"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SPIDER, L"Паук"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HELICOPTER, L"Вертолет"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::AIRCARRIER, L"Авианосец"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BIG_TANK, L"Большой танк"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PLANE, L"Самолет"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TELEPORT, L"Телепорт"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::DOWN_TOWER, L"Ухудшение башни"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ANT, L"Муравей"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::COW, L"Корова"));

	//manual
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SPEED, L"Скорость"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SIZE, L"Размер"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ARMOR, L"Броня"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SMALL, L"Мелкий"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::MID, L"Средний"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BIG, L"Крупный"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SLOW, L"Медленный"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FAST, L"Быстрая"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NEXT, L"Вперед"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CREDITS, L"Авторы"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BACK, L"Вернуться"));

	//difficult
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EASY, L"Легко"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NORMAL, L"Нормально"));
	russainTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HARD, L"Трудно"));





	//menu
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::MANUAL, L"Manual"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT, L"Exit"));

	//settings
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::SETTINGS, L"Settings"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::AUDIO, L"Audio"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::SOUND, L"Sound"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::MUSIC, L"Music"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::VIDEO, L"Video"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::FULLSCREEN, L"Fullscreen"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::RESOLUTION, L"Resolution"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::MISC, L"Misc"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::LANGUAGE, L"Language"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::ACCEPT, L"Accept"));
	englishTranslation.insert(pair<int, wstring>(TranslationsManager::CANCEL, L"Cancel"));

	//panel
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"Venom"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"Bomb"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Freeze bomb"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Speed up tower"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Strengthen tower"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_STOP, L"Stop all"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_BASE, L"Tower base"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_POWER, L"Tower power"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"Tower rocket"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"Tower freeze"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_LASER, L"Tower laser"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"Tower improved"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::LEVEL, L"Level"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Damage per seconds"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::RADIUS, L"Radius"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::KILLS, L"Kills"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::COST, L"Cost"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SELL_COST, L"Sell cost"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::WAVE, L"Wave"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Has a cumulative effect of reducing enemy armor"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Slows down the enemy"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"Produces resources"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Rocket explosion deals area damage"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attacks continuously"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Most effective in all stats"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Deals damage instantly"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Slows down enemies instantly"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"Continuously deals damage in an area."));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Temporarily increases tower damage"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Temporarily increases tower attack speed"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stops the movement of all enemies"));

	//game
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PAUSED, L"Paused"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CONTINUE, L"Continue"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::RESTART, L"Restart"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"Exit to menu"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Exit from game"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::GAME_OVER, L"Game Over!"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations!"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::START_GAME, L"Press Space to start"));

	//instructions
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Welcome!"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Towers"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"Abilities"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Money"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Press Return to continue or Space to skip"));

	//enemies
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SCORPION, L"Башни"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CAR, L"Машина"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TANK, L"Танк"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SPIDER, L"Паук"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HELICOPTER, L"Вертолет"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::AIRCARRIER, L"Авианосец"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BIG_TANK, L"Большой танк"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PLANE, L"Самолет"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TELEPORT, L"Телепорт"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::DOWN_TOWER, L"Ухудшение башни"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ANT, L"Муравей"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::COW, L"Корова"));

	//manual
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SPEED, L"Speed"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SIZE, L"Size"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ARMOR, L"Armour"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HEALTH, L"Health"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY, L"Abilities"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SMALL, L"Small"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::MID, L"Mid"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BIG, L"Big"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SLOW, L"Slow"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"Normal"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FAST, L"Fast"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NEXT, L"Next"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PREVIOUS, L"Previous"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CREDITS, L"Credits"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BACK, L"Back"));

	//difficult
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EASY, L"Easy"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NORMAL, L"Normal"));
	englishTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HARD, L"Hard"));


	//menu
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::MANUAL, L"Manual"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CREDITS, L"Crédits"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT, L"Sortir du jeu"));

	//settings
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::SETTINGS, L"Paramètres"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::AUDIO, L"Audio"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::SOUND, L"Sons"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::MUSIC, L"Musique"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::VIDEO, L"Vidéo"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::FULLSCREEN, L"Plein écran"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::RESOLUTION, L"Résolution"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::MISC, L"Autres"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::LANGUAGE, L"Langue"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::ACCEPT, L"Confirmé"));
	frenchTranslation.insert(pair<int, wstring>(TranslationsManager::CANCEL, L"Rejet"));

	//panel
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_VENOM, L"Acide"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_BOMB, L"Bombe"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_FREEZE_BOMB, L"Bombe-glace"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_ATTACK_SPEED, L"Tour à vitesse d'attaque augmentée"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_INCREASE_TOWER_DAMAGE, L"Tour aux dommages augmentés"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY_STOP, L"Stopper"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_BASE, L"Tour de base"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_POWER, L"Tour de production d'énergie"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_ROCKET, L"Tour réacteur-fusée"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_FREEZE, L"Tour à obus glaçants"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_LASER, L"Tour laser"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TOWER_IMPROVED, L"Tour de base améliorée"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::LEVEL, L"Niveau"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::DAMAGE_PER_SECOND, L"Dommages"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::RADIUS, L"Rayon"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::KILLS, L"Ennemis tués"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::COST, L"Prix"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SELL_COST, L"Prix de la vente"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::WAVE, L"Vague"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BASE_TOWER_DESCRIPTION, L"Effet cumulatif de réduire la cuirasse d'un ennemi"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FREEZE_TOWER_DESCRIPTION, L"Ralentissement d'un ennemi"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::POWER_TOWER_DESCRIPTION, L"Extraire les ressourses"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ROCKET_TOWER_DESCRIPTION, L"Dommage d'une certaine territoire"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::LASER_TOWER_DESCRIPTION, L"Attaquer continuellement"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::IMPROVED_TOWER_DESCRIPTION, L"Attaquer avec les meilleurs indices"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BOMB_ABILITY_DESCRIPTION, L"Exploision instantanée"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FREEZE_BOMB_ABILITY_DESCRIPTION, L"Ralentissement instantané d'un ennemi"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::VENOM_ABILITY_DESCRIPTION, L"Dommages prolongé d'une certaine territoire"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INC_DMG_ABILITY_DESCRIPTION, L"Amélioration temporaire des dommages causés"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INC_AS_ABILITY_DESCRIPTION, L"Accélération temporaire des tirs"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::STOP_ABILITY_DESCRIPTION, L"Stopper tous les ennemis"));

	//game
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PAUSED, L"Pause"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CONTINUE, L"Continuer"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::RESTART, L"Recommencer"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT_TO_MENU, L"Sortir au menu"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EXIT_FROM_GAME, L"Sortir du jeu"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::GAME_OVER, L"Tu as perdu, désolé"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CONGRATULATIONS, L"Congratulations"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::START_GAME, L"Tapez l'Espace afin de commencer le jeu"));

	//instructions
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_WELCOME, L"Welcome!"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_TOWERS, L"Towers"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_ABILITIES, L"Abilities"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_MONEY, L"Money"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_HEALTH, L"Health"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_PROGRESS, L"Progress"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::INSTRUCTION_SKIP, L"Taper la Touche d'entrée pour continuer ou l'Espace pour passer"));

	//enemies
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SCORPION, L"Башни"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CAR, L"Машина"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TRICYCLE, L"Трицикл"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TANK, L"Танк"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SPIDER, L"Паук"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HELICOPTER, L"Вертолет"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::AIRCARRIER, L"Авианосец"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BIG_TANK, L"Большой танк"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PLANE, L"Самолет"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::REPAIR, L"Скорая"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SHELL, L"Ракушка"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::TELEPORT, L"Телепорт"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SELF_HEAL, L"Самолечит"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::DOWN_TOWER, L"Ухудшение башни"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ANT, L"Муравей"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::COW, L"Корова"));

	//manual
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SPEED, L"Скорость"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SIZE, L"Размер"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ARMOR, L"Броня"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HEALTH, L"Здоровье"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::ABILITY, L"Особые способности"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SMALL, L"Мелкий"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::MID, L"Средний"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BIG, L"Крупный"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::SLOW, L"Медленный"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NORMAL_SPEED, L"Нормальная"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::FAST, L"Быстрая"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NEXT, L"Вперед"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::PREVIOUS, L"Назад"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::CREDITS, L"Авторы"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::BACK, L"Вернуться"));

	//difficult
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::EASY, L"Easy"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::NORMAL, L"Normal"));
	frenchTranslation.insert(pair<int, wstring>(GAME_TRANSLATION::HARD, L"Hard"));


	Engine::Instance().translationsManager()->addTranslation("rus", russainTranslation);
	Engine::Instance().translationsManager()->addTranslation("eng", englishTranslation);
	Engine::Instance().translationsManager()->addTranslation("fr", frenchTranslation);
}
