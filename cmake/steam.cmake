set(STEAM_DIR steamsdk CACHE STRING "Steam sdk dir")

target_include_directories(${PROJECT_NAME} PUBLIC ${STEAM_DIR}/public)

set(STEAM_LIB_NAME)
set(STEAM_LIB_PATH)

if (WIN32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(STEAM_LIB_NAME steam_api64)
    set(STEAM_LIB_PATH win64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(STEAM_LIB_NAME steam_api)
    set(STEAM_LIB_PATH)
endif()
endif(WIN32)

if(APPLE)
    set(STEAM_LIB_NAME steam_api)
    set(STEAM_LIB_PATH osx)
endif()

if(UNIX AND NOT APPLE)
    set(STEAM_LIB_NAME steam_api)
    set(STEAM_LIB_PATH linux64)
endif()

target_link_directories(${PROJECT_NAME} PUBLIC ${STEAM_DIR}/redistributable_bin/${STEAM_LIB_PATH})
target_link_libraries(${PROJECT_NAME} ${STEAM_LIB_NAME})
