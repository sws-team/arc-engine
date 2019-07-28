HEADERS += \
    $$PWD/gameobject.h \
    $$PWD/effects.h \
    $$PWD/gamedrawable.h \
    $$PWD/gamepanel.h \
    $$PWD/leveldef.h

SOURCES += \
    $$PWD/gameobject.cpp \
    $$PWD/effects.cpp \
    $$PWD/gamepanel.cpp

include($$PWD/Audio/Audio.pri)
include($$PWD/collisions/collisions.pri)
include($$PWD/Level/Level.pri)
