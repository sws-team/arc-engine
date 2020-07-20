INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/Windows
INCLUDEPATH += $$PWD/include/Widgets

CONFIG(release, debug|release): LIBS += -L$$PWD/lib -lEngine
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib -lEngined
