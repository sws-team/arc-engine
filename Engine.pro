TEMPLATE = lib
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib
DESTDIR = $$PWD/lib

include($$PWD/../../build_environment.pri)

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/Windows
INCLUDEPATH += $$PWD/include/Widgets

DEFINES += ENGINE_LIBRARY

HEADERS += \
    include/Widgets/button.h \
    include/Widgets/checkbox.h \
    include/Widgets/chooselist.h \
    include/Widgets/valuescale.h \
    include/Widgets/widget.h \
    include/Windows/aboutwindow.h \
    include/Windows/closewindow.h \
    include/Windows/introwindow.h \
    include/Windows/menu.h \
    include/Windows/settingswindow.h \
    include/enginedef.h \
    include/gameobject.h \
    include/managers.h \
    include/collisions.h \
    include/mainwindow.h \
    include/stdheader.h \
    include/timer.h \
    include/statewindow.h \
    include/engine.h \

SOURCES += \
    src/Widgets/button.cpp \
    src/Widgets/checkbox.cpp \
    src/Widgets/chooselist.cpp \
    src/Widgets/valuescale.cpp \
    src/Widgets/widget.cpp \
    src/Windows/aboutwindow.cpp \
    src/Windows/closewindow.cpp \
    src/Windows/introwindow.cpp \
    src/Windows/menu.cpp \
    src/Windows/settingswindow.cpp \
    src/managers.cpp \
    src/gameobject.cpp \
    src/mainwindow.cpp \
    src/collisions.cpp \
    src/engine.cpp \
    src/statewindow.cpp \
    src/timer.cpp

GIT_VERSION = $$system($$quote(git describe))
QMAKE_SUBSTITUTES += $$PWD/version.h.in
DISTFILES += version.h.in

NORMAL_VERSION = $$replace(GIT_VERSION, "-[a-z|0-9]+", "" )
VERSION = $${NORMAL_VERSION}
DEFINES += ENGINE_VERSION=\\\"$${NORMAL_VERSION}\\\"

win32: DEFINES += OS_WIN
unix: DEFINES += OS_UNIX
macx{
DEFINES += OS_MAC
DEFINES -= OS_UNIX
}

CONFIG(release, debug|release): TARGET = Engine
CONFIG(debug, debug|release): TARGET = Engined

include($$PWD/../../3rdParty/SFML.pri)
