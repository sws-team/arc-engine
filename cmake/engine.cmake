find_package (Git)
if (GIT_FOUND)
    message("git found: ${GIT_EXECUTABLE}, version: ${GIT_VERSION_STRING}")
  else()
    message(SEND_ERROR "set up git")
endif (GIT_FOUND)

execute_process(
     COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_LIST_DIR} describe
     OUTPUT_VARIABLE ENGINE_VERSION
 )
STRING(REGEX REPLACE "-[a-z|0-9]+" "" ENGINE_NORMAL_VERSION ${ENGINE_VERSION})
STRING(REGEX REPLACE "\n$" "" ENGINE_NORMAL_VERSION ${ENGINE_NORMAL_VERSION})
add_definitions("-DENGINE_VERSION=\"${ENGINE_NORMAL_VERSION}\"")

set(ARC_ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

include_directories(${ARC_ENGINE_DIR}/include)
include_directories(${ARC_ENGINE_DIR}/include/Windows)
include_directories(${ARC_ENGINE_DIR}/include/Widgets)

set(ARC_ENGINE_SOURCES
    ${ARC_ENGINE_DIR}/src/Widgets/button.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/checkbox.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/chooselist.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/valuescale.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/widget.cpp
    ${ARC_ENGINE_DIR}/src/Windows/aboutwindow.cpp
    ${ARC_ENGINE_DIR}/src/Windows/closewindow.cpp
    ${ARC_ENGINE_DIR}/src/Windows/introwindow.cpp
    ${ARC_ENGINE_DIR}/src/Windows/menu.cpp
    ${ARC_ENGINE_DIR}/src/managers.cpp
    ${ARC_ENGINE_DIR}/src/gameobject.cpp
    ${ARC_ENGINE_DIR}/src/mainwindow.cpp
    ${ARC_ENGINE_DIR}/src/collisions.cpp
    ${ARC_ENGINE_DIR}/src/engine.cpp
    ${ARC_ENGINE_DIR}/src/statewindow.cpp
    ${ARC_ENGINE_DIR}/src/timer.cpp
    ${ARC_ENGINE_DIR}/src/utils.cpp
    )
