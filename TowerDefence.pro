GIT_ENABLED = $$system($$quote(git version))
isEmpty( GIT_ENABLED ) {
error( "set up git" )
}

TEMPLATE = app
CONFIG += c++17
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = TowerDefence

CONFIG += c++14

include(../build_environment.pri)
include($$PWD/3rdPartyIncludes.pri)

SOURCES += main.cpp \
    globalvariables.cpp \
    mainwindow.cpp \
    settings.cpp \
    controller.cpp \
    statewindow.cpp \
    gamewindow.cpp \
    timer.cpp
HEADERS += \
    globalvariables.h \
    mainwindow.h \
    settings.h \
    stdheader.h \
    graphics.h \
    controller.h \
    controlsettings.h \
    timer.h \
    statewindow.h \
    gamewindow.h


GIT_VERSION = $$system($$quote(git describe))
QMAKE_SUBSTITUTES += $$PWD/version.h.in
DISTFILES += version.h.in

NORMAL_VERSION = $$replace(GIT_VERSION, "-[a-z|0-9]+", "" )
VERSION = $${NORMAL_VERSION}
DEFINES += APP_VERSION=\\\"$${NORMAL_VERSION}\\\"

win32: DEFINES += OS_WIN
unix: DEFINES += OS_UNIX

TARGET = TowerDefence_$${VERSION}

include($$PWD/Windows/Windows.pri)
include($$PWD/Game/Game.pri)
include($$PWD/Engine/Engine.pri)
include($$PWD/Translations/Translations.pri)
include($$PWD/ResourcesManager/ResourcesManager.pri)
include($$PWD/Crypto/Crypto.pri)

#DEFINES += TEST_BUILD

contains(DEFINES, TEST_BUILD) {
QMAKE_LFLAGS += -g
QMAKE_CXXFLAGS += -g
TARGET = TowerDefence_$${VERSION}_test
}
