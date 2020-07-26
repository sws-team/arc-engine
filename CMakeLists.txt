#config

if(WIN32) #if(MSVC OR MSYS OR MINGW)
    add_definitions("-DOS_WIN")
endif()

if(UNIX AND NOT APPLE)
    add_definitions("-DOS_UNIX")
endif()

if(APPLE)
    add_definitions("-DOS_MAC")
endif()

#engine
add_definitions("-DENGINE_VERSION=\"1.0.0.5\"")

set(ARC_ENGINE_PATH ${CMAKE_CURRENT_LIST_DIR})

include_directories(${ARC_ENGINE_PATH}/include)
include_directories(${ARC_ENGINE_PATH}/include/Windows)
include_directories(${ARC_ENGINE_PATH}/include/Widgets)

file(GLOB SOURCES
    ${SOURCES}
    ${ARC_ENGINE_PATH}/src/Widgets/button.cpp
    ${ARC_ENGINE_PATH}/src/Widgets/checkbox.cpp
    ${ARC_ENGINE_PATH}/src/Widgets/chooselist.cpp
    ${ARC_ENGINE_PATH}/src/Widgets/valuescale.cpp
    ${ARC_ENGINE_PATH}/src/Widgets/widget.cpp
    ${ARC_ENGINE_PATH}/src/Windows/aboutwindow.cpp
    ${ARC_ENGINE_PATH}/src/Windows/closewindow.cpp
    ${ARC_ENGINE_PATH}/src/Windows/introwindow.cpp
    ${ARC_ENGINE_PATH}/src/Windows/menu.cpp
    ${ARC_ENGINE_PATH}/src/managers.cpp
    ${ARC_ENGINE_PATH}/src/gameobject.cpp
    ${ARC_ENGINE_PATH}/src/mainwindow.cpp
    ${ARC_ENGINE_PATH}/src/collisions.cpp
    ${ARC_ENGINE_PATH}/src/engine.cpp
    ${ARC_ENGINE_PATH}/src/statewindow.cpp
    ${ARC_ENGINE_PATH}/src/timer.cpp    
    )
