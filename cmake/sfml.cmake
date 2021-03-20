set(SFML_DIR SFML-2.5.1-build CACHE STRING "SFML build dir")

find_package(SFML 2.5 COMPONENTS graphics audio system window REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system sfml-window)
target_link_libraries(${PROJECT_NAME} opengl32)
