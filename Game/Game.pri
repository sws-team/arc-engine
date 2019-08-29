HEADERS += \
    $$PWD/animation.h \
    $$PWD/gameobject.h \
    $$PWD/gamedrawable.h \
    $$PWD/gamepanel.h \
    $$PWD/leveldef.h

SOURCES += \
    $$PWD/animation.cpp \
    $$PWD/gameobject.cpp \
    $$PWD/gamepanel.cpp

include($$PWD/Audio/Audio.pri)
include($$PWD/collisions/collisions.pri)
include($$PWD/Level/Level.pri)
