set(STEAM_DIR steamsdk CACHE STRING "Steam sdk dir")

target_include_directories(${PROJECT_NAME} PUBLIC ${STEAM_DIR}/public)

if (WIN32)
target_link_directories(${PROJECT_NAME} PUBLIC ${STEAM_DIR}/redistributable_bin/win64)
target_link_libraries(${PROJECT_NAME} steam_api64)
endif (WIN32)

if(APPLE)
    # for MacOS X or iOS, watchOS, tvOS (since 3.10.3)
endif()

if(UNIX AND NOT APPLE)
    target_link_directories(${PROJECT_NAME} PUBLIC ${STEAM_DIR}/redistributable_bin/linux64)
    target_link_libraries(${PROJECT_NAME} steam_api)
endif()
