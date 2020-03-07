GIT_ENABLED = $$system($$quote(git version))
isEmpty( GIT_ENABLED ) {
error( "set up git" )
}

TEMPLATE = app
CONFIG += windows c++11
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = TowerDefence
#QMAKE_CXXFLAGS += -std=c++17

include(../build_environment.pri)
include($$PWD/3rdPartyIncludes.pri)

SOURCES += main.cpp \
    gamemanagers.cpp \
    gameoptions.cpp \
    gamestatemanager.cpp \
    controller.cpp
HEADERS += \
    gamemanagers.h \
    gameoptions.h \
    gamestatemanager.h \
    controller.h \
    tile.h

GIT_VERSION = $$system($$quote(git describe))
QMAKE_SUBSTITUTES += $$PWD/version.h.in
DISTFILES += version.h.in

NORMAL_VERSION = $$replace(GIT_VERSION, "-[a-z|0-9]+", "" )
VERSION = $${NORMAL_VERSION}
DEFINES += APP_VERSION=\\\"$${NORMAL_VERSION}\\\"

win32: DEFINES += OS_WIN
unix: DEFINES += OS_UNIX

DEFINES += STEAM_API

TARGET = TowerDefence

include($$PWD/../Engine/Engine/Engine.pri)
include($$PWD/../Engine/ResourcesLib/ResourcesLib.pri)
include($$PWD/../Engine/GameAchievements/GameAchievements.pri)

include($$PWD/Windows/Windows.pri)
include($$PWD/Game/Game.pri)

contains(DEFINES, STEAM_API) {
include($$PWD/../3rdParty/SteamAPI.pri)
}

RC_ICONS = icon.ico
