GIT_ENABLED = $$system($$quote(git version))
isEmpty( GIT_ENABLED ) {
error( "set up git" )
}

TEMPLATE = app
CONFIG += windows c++11
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = bin
TARGET = ArcDefence
#QMAKE_CXXFLAGS += -std=c++17

include(../build_environment.pri)

SOURCES += main.cpp \
    gamemanagers.cpp \
    gameoptions.cpp \
    gamestatemanager.cpp \
    controller.cpp \
    gametranslations.cpp \
    loading.cpp
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

win32:DEFINES += OS_WIN
unix: DEFINES += OS_UNIX

DEFINES += STEAM_API

include($$PWD/../Engine/Engine/Engine.pri)
include($$PWD/3rdPartyIncludes.pri)
include($$PWD/../Engine/ResourcesLib/ResourcesLib.pri)
include($$PWD/../Engine/GamePlatform/GamePlatform.pri)

include($$PWD/Windows/Windows.pri)
include($$PWD/Game/Game.pri)

contains(DEFINES, STEAM_API) {
include($$PWD/../3rdParty/SteamAPI.pri)
}

win32{
LIBS += user32.lib
RC_ICONS = icon.ico
}

unix:!macx {
LIBS += -lX11
QMAKE_RPATHDIR += $ORIGIN/libs
}

macx: {
QMAKE_RPATHDIR += $ORIGIN/frameworks
QMAKE_LFLAGS += -Wl,-rpath,@executable_path/frameworks
#ICON = icon.icns
}

#DEFINES += DEMO_VERSION
DEFINES += RELEASE_BUILD

#DEFINES += LOAD_MAPS
#DEFINES += LOAD_BALANCE

contains(DEFINES, DEMO_VERSION){
TARGET = ArcDefenceDemo
win32{
RC_ICONS = demo_icon.ico
}
}
