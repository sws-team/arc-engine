#ifndef SOUNDSDEF_H
#define SOUNDSDEF_H

#include <string>
#include <vector>

const std::string INTRO_SOUND_FILE = "sounds/intro.ogg";
const std::string MENU_SOUND_FILE = "sounds/main.ogg";
const std::string CREDITS_SOUND_FILE = "sounds/credits.ogg";

const std::string UPGRADE_SOUND_FILE = "sounds/upgrade.ogg";
const std::string WIN_SOUND_FILE = "sounds/win.ogg";
const std::string WARNING_SOUND_FILE = "sounds/warning.ogg";
const std::string SETUP_SOUND_FILE = "sounds/setup.ogg";
const std::string SELL_SOUND_FILE = "sounds/sell.ogg";
const std::string TARGET_LOCK_SOUND_FILE = "sounds/target_lock.ogg";
const std::string TOWER_EXPLOSION_SOUND_FILE = "sounds/tower_explosion.ogg";
//ui
const std::string HOVER_SOUND_FILE = "sounds/ui/hover.ogg";
const std::string CLICK_SOUND_FILE = "sounds/ui/click.ogg";
//enemies
const std::string CATCH_SOUND_FILE = "sounds/enemies/catch.ogg";
//abilities
const std::string BOMB_SOUND_FILE = "sounds/abilities/bomb.ogg";
const std::string STOP_SOUND_FILE = "sounds/abilities/stop.ogg";
//soundtracks
const std::vector<std::string> TRACKS_FILES = {
	{"sounds/music/track_1.ogg"},
	{"sounds/music/track_2.ogg"},
	{"sounds/music/track_3.ogg"},
	{"sounds/music/track_4.ogg"},
	{"sounds/music/track_5.ogg"},
	{"sounds/music/track_6.ogg"},
	{"sounds/music/track_7.ogg"},
	{"sounds/music/track_8.ogg"}
};
const std::vector<std::string> FINAL_TRACKS_FILES = {
	{"sounds/music/final_1.ogg"},
	{"sounds/music/final_2.ogg"},
	{"sounds/music/final_3.ogg"}
};


#endif // SOUNDSDEF_H
