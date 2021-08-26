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

option(ARC_DEBUG OFF)

#resources
include(${CMAKE_CURRENT_LIST_DIR}/../Resources/arcresources.cmake)

set(ARC_ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

include_directories(${ARC_ENGINE_DIR}/include)
include_directories(${ARC_ENGINE_DIR}/include/Windows)
include_directories(${ARC_ENGINE_DIR}/include/Widgets)
include_directories(${ARC_ENGINE_DIR}/include/Objects)

set(ARC_ENGINE_SOURCES
    ${ARC_ENGINE_DIR}/src/Widgets/button.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/checkbox.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/chooselist.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/valuescale.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/widget.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arclabel.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arcbutton.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arctilemap.cpp

    ${ARC_ENGINE_DIR}/src/Windows/aboutwindow.cpp
    ${ARC_ENGINE_DIR}/src/Windows/closewindow.cpp
    ${ARC_ENGINE_DIR}/src/Windows/introwindow.cpp
    ${ARC_ENGINE_DIR}/src/Windows/menu.cpp

    ${ARC_ENGINE_DIR}/src/Objects/arcsprite.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arcanimatedsprite.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arcskeletonanimation.cpp

    ${ARC_ENGINE_DIR}/src/arcobject.cpp
    ${ARC_ENGINE_DIR}/src/arcdebug.cpp
    ${ARC_ENGINE_DIR}/src/arcwindow.cpp
    ${ARC_ENGINE_DIR}/src/arcaction.cpp
    ${ARC_ENGINE_DIR}/src/managers.cpp
    ${ARC_ENGINE_DIR}/src/gameobject.cpp
    ${ARC_ENGINE_DIR}/src/mainwindow.cpp
    ${ARC_ENGINE_DIR}/src/collisions.cpp
    ${ARC_ENGINE_DIR}/src/engine.cpp
    ${ARC_ENGINE_DIR}/src/timer.cpp
    ${ARC_ENGINE_DIR}/src/utils.cpp

    ${RESOURCES_SOURCES}
    )

#3rd party
set(3RD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/../3rdParty)

include_directories(${3RD_PARTY_DIR}/rapidjson/include)

include(${3RD_PARTY_DIR}/DragonBonesSFML/DragonBones.cmake)

set(ARC_ENGINE_SOURCES
    ${ARC_ENGINE_SOURCES}
    ${DRAGON_BONES_SOURCES}
    )

if (ARC_DEBUG)
    add_definitions(-DARC_DEBUG)
    include_directories(${3RD_PARTY_DIR}/imgui)
    include_directories(${3RD_PARTY_DIR}/imgui-sfml)

    set(IMGUI_SOURCES
	${3RD_PARTY_DIR}/imgui/imgui.cpp
	${3RD_PARTY_DIR}/imgui/imgui_draw.cpp
	${3RD_PARTY_DIR}/imgui/imgui_tables.cpp
	${3RD_PARTY_DIR}/imgui/imgui_widgets.cpp
	${3RD_PARTY_DIR}/imgui/imgui_demo.cpp

	${3RD_PARTY_DIR}/imgui-sfml/imgui-SFML.cpp
	)
    set(ARC_ENGINE_SOURCES
	${ARC_ENGINE_SOURCES}
	${IMGUI_SOURCES}
	)
endif(ARC_DEBUG)

