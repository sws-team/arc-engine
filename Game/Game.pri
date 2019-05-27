HEADERS += \
    $$PWD/gameobject.h \
    $$PWD/effects.h \
    $$PWD/gamedrawable.h \
    $$PWD/gamepanel.h \
    $$PWD/lifebar.h \
    $$PWD/gamestate.h

SOURCES += \
    $$PWD/gameobject.cpp \
    $$PWD/effects.cpp \
    $$PWD/gamepanel.cpp \
    $$PWD/lifebar.cpp

include($$PWD/Audio/Audio.pri)
include($$PWD/collisions/collisions.pri)
include($$PWD/Level/Level.pri)
