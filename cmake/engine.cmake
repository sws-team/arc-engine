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

add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

#resources
include(${CMAKE_CURRENT_LIST_DIR}/../Resources/arcresources.cmake)

set(ARC_ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

include_directories(${ARC_ENGINE_DIR}/include)
include_directories(${ARC_ENGINE_DIR}/src)
include_directories(${ARC_ENGINE_DIR}/src/Scenes)
include_directories(${ARC_ENGINE_DIR}/src/Widgets)
include_directories(${ARC_ENGINE_DIR}/src/Objects)

set(ARC_ENGINE_HEADERS
    ${ARC_ENGINE_DIR}/src/Widgets/arclabel.h
    ${ARC_ENGINE_DIR}/src/Widgets/arcbutton.h
    ${ARC_ENGINE_DIR}/src/Widgets/arctilemap.h
    ${ARC_ENGINE_DIR}/src/Widgets/customwidgets.h
    ${ARC_ENGINE_DIR}/src/Widgets/arcscrollarea.h
    ${ARC_ENGINE_DIR}/src/Widgets/arccheckbox.h

    ${ARC_ENGINE_DIR}/src/Scenes/aboutscene.h
    ${ARC_ENGINE_DIR}/src/Scenes/closescene.h
    ${ARC_ENGINE_DIR}/src/Scenes/introscene.h
    ${ARC_ENGINE_DIR}/src/Scenes/menuscene.h

    ${ARC_ENGINE_DIR}/src/Objects/arcsprite.h
    ${ARC_ENGINE_DIR}/src/Objects/arcanimatedsprite.h
    ${ARC_ENGINE_DIR}/src/Objects/arcskeletonanimation.h
    ${ARC_ENGINE_DIR}/src/Objects/arclayout.h
    ${ARC_ENGINE_DIR}/src/Objects/arcpolygon.h
    ${ARC_ENGINE_DIR}/src/Objects/arcrect.h
    ${ARC_ENGINE_DIR}/src/Objects/navigationmap.h

    ${ARC_ENGINE_DIR}/src/arclog.h
    ${ARC_ENGINE_DIR}/src/arcobject.h
    ${ARC_ENGINE_DIR}/src/arcdebug.h
    ${ARC_ENGINE_DIR}/src/arcwindow.h
    ${ARC_ENGINE_DIR}/src/arcscene.h
    ${ARC_ENGINE_DIR}/src/arcaction.h
    ${ARC_ENGINE_DIR}/src/managers.h
    ${ARC_ENGINE_DIR}/src/mainwindow.h
    ${ARC_ENGINE_DIR}/src/collisions.h
    ${ARC_ENGINE_DIR}/src/engine.h
    ${ARC_ENGINE_DIR}/src/arctimer.h
    ${ARC_ENGINE_DIR}/src/arcengineutils.h
    ${ARC_ENGINE_DIR}/src/arcproperties.h
    ${ARC_ENGINE_DIR}/src/arcvariant.h
    ${ARC_ENGINE_DIR}/src/tutorial.h
    )

set(ARC_ENGINE_SOURCES
    ${ARC_ENGINE_DIR}/src/Widgets/arclabel.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arcbutton.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arctilemap.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/customwidgets.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arcscrollarea.cpp
    ${ARC_ENGINE_DIR}/src/Widgets/arccheckbox.cpp

    ${ARC_ENGINE_DIR}/src/Scenes/aboutscene.cpp
    ${ARC_ENGINE_DIR}/src/Scenes/closescene.cpp
    ${ARC_ENGINE_DIR}/src/Scenes/introscene.cpp
    ${ARC_ENGINE_DIR}/src/Scenes/menuscene.cpp

    ${ARC_ENGINE_DIR}/src/Objects/arcsprite.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arcanimatedsprite.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arcskeletonanimation.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arclayout.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arcpolygon.cpp
    ${ARC_ENGINE_DIR}/src/Objects/arcrect.cpp
    ${ARC_ENGINE_DIR}/src/Objects/navigationmap.cpp

    ${ARC_ENGINE_DIR}/src/arcobject.cpp
    ${ARC_ENGINE_DIR}/src/arcdebug.cpp
    ${ARC_ENGINE_DIR}/src/arcwindow.cpp
    ${ARC_ENGINE_DIR}/src/arcscene.cpp
    ${ARC_ENGINE_DIR}/src/arcaction.cpp
    ${ARC_ENGINE_DIR}/src/managers.cpp
    ${ARC_ENGINE_DIR}/src/mainwindow.cpp
    ${ARC_ENGINE_DIR}/src/collisions.cpp
    ${ARC_ENGINE_DIR}/src/engine.cpp
    ${ARC_ENGINE_DIR}/src/arctimer.cpp
    ${ARC_ENGINE_DIR}/src/arcengineutils.cpp
    ${ARC_ENGINE_DIR}/src/arcproperties.cpp
    ${ARC_ENGINE_DIR}/src/arcvariant.cpp
    ${ARC_ENGINE_DIR}/src/tutorial.cpp

    ${RESOURCES_SOURCES}
    )

#3rd party
set(3RD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/../3rdParty)

include_directories(${3RD_PARTY_DIR}/rapidjson/include)
include_directories(${3RD_PARTY_DIR}/tinypile)

include(${3RD_PARTY_DIR}/DragonBonesSFML/DragonBones.cmake)

set(ARC_ENGINE_SOURCES
    ${ARC_ENGINE_HEADERS}
    ${ARC_ENGINE_SOURCES}
    ${DRAGON_BONES_SOURCES}
    )

if (ARC_DEBUG)
    add_definitions(-DARC_DEBUG)
    include_directories(${3RD_PARTY_DIR}/imgui)
    include_directories(${3RD_PARTY_DIR}/imgui/misc/cpp)
    include_directories(${3RD_PARTY_DIR}/imgui-sfml)

    set(IMGUI_SOURCES
	${3RD_PARTY_DIR}/imgui/imgui.cpp
	${3RD_PARTY_DIR}/imgui/imgui_draw.cpp
	${3RD_PARTY_DIR}/imgui/imgui_tables.cpp
	${3RD_PARTY_DIR}/imgui/imgui_widgets.cpp
	${3RD_PARTY_DIR}/imgui/imgui_demo.cpp
	${3RD_PARTY_DIR}/imgui/misc/cpp/imgui_stdlib.cpp

	${3RD_PARTY_DIR}/imgui-sfml/imgui-SFML.cpp
	)
    set(ARC_ENGINE_SOURCES
	${ARC_ENGINE_SOURCES}
	${IMGUI_SOURCES}
	)
endif(ARC_DEBUG)

