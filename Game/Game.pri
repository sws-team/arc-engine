HEADERS += \
    $$PWD/gameobject.h \
    $$PWD/level.h \
    $$PWD/effects.h \
    $$PWD/gamedrawable.h \
    $$PWD/gamepanel.h \
    $$PWD/mission.h \
    $$PWD/cpu.h \
    $$PWD/gamestate.h \
    $$PWD/objectsfactory.h

SOURCES += \
    $$PWD/gameobject.cpp \
    $$PWD/level.cpp \
    $$PWD/effects.cpp \
    $$PWD/gamepanel.cpp \
    $$PWD/mission.cpp \
    $$PWD/cpu.cpp \
    $$PWD/objectsfactory.cpp

include($$PWD/Audio/Audio.pri)
include($$PWD/Locations/Locations.pri)
include($$PWD/collisions/collisions.pri)
include($$PWD/Level/level.pri)
