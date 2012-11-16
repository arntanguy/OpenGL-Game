SET(PROJECT_NAME Game)
SET(BIN_DIR .)
SET(SRC_DIR .)
SET(INCLUDE_DIR .)
SET(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.6)
#set(CMAKE_CXX_FLAGS "-Wall -W -Werror -ansi -pedantic -g")

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CAL3D REQUIRED)
find_package(SFML COMPONENTS system window network audio graphics REQUIRED)

### DEFINE NEEDED SOURCES
SET(SOURCES
${SRC_DIR}/main.cpp
${SRC_DIR}/viewer.cpp
${SRC_DIR}/tga.cpp
${SRC_DIR}/tick.cpp
${SRC_DIR}/TerrainPage.cpp
${SRC_DIR}/RessourcesManager.cpp
)

include_directories (${SRC_DIR} ${OPENGL_INCLUDE_DIR} ${CAL3D_INCLUDE_DIR} ${INCLUDE_DIR})

#### CREATE EXECUTABLE
add_executable (
   ${BIN_DIR}/${PROJECT_NAME} ${SOURCES}
)

target_link_libraries(${BIN_DIR}/${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${CAL3D_LIBRARY} ${SFML_LIBRARIES})
