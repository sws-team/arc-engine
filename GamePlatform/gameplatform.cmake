option(STEAM_PLATFORM ON)
option(GOOGLE_PLAY_PLATFORM OFF)

set(GAMEPLATFORM_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/gameplatform.cpp
    )

include_directories(${CMAKE_CURRENT_LIST_DIR})

if(STEAM_PLATFORM)
    list(APPEND GAMEPLATFORM_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/steamplatform.cpp
	${CMAKE_CURRENT_LIST_DIR}/steamplatform/lobby.cpp
	${CMAKE_CURRENT_LIST_DIR}/steamplatform/network.cpp
	)
    add_definitions(-DSTEAM_PLATFORM)
    add_definitions(-DGAME_PLATFORM=SteamPlatform)
endif(STEAM_PLATFORM)
