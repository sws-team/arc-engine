INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/Windows
INCLUDEPATH += $$PWD/include/Widgets

#DEFINES += ENGINE_LIBRARY

HEADERS += \
    $$PWD/include/Widgets/button.h \
    $$PWD/include/Widgets/checkbox.h \
    $$PWD/include/Widgets/chooselist.h \
    $$PWD/include/Widgets/valuescale.h \
    $$PWD/include/Widgets/widget.h \
    $$PWD/include/Windows/aboutwindow.h \
    $$PWD/include/Windows/closewindow.h \
    $$PWD/include/Windows/introwindow.h \
    $$PWD/include/Windows/menu.h \
    $$PWD/include/Windows/settingswindow.h \
    $$PWD/include/enginedef.h \
    $$PWD/include/gameobject.h \
    $$PWD/include/managers.h \
    $$PWD/include/collisions.h \
    $$PWD/include/mainwindow.h \
    $$PWD/include/stdheader.h \
    $$PWD/include/timer.h \
    $$PWD/include/statewindow.h \
    $$PWD/include/engine.h \

SOURCES += \
    $$PWD/src/Widgets/button.cpp \
    $$PWD/src/Widgets/checkbox.cpp \
    $$PWD/src/Widgets/chooselist.cpp \
    $$PWD/src/Widgets/valuescale.cpp \
    $$PWD/src/Widgets/widget.cpp \
    $$PWD/src/Windows/aboutwindow.cpp \
    $$PWD/src/Windows/closewindow.cpp \
    $$PWD/src/Windows/introwindow.cpp \
    $$PWD/src/Windows/menu.cpp \
    $$PWD/src/Windows/settingswindow.cpp \
    $$PWD/src/managers.cpp \
    $$PWD/src/gameobject.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/collisions.cpp \
    $$PWD/src/engine.cpp \
    $$PWD/src/statewindow.cpp \
    $$PWD/src/timer.cpp

