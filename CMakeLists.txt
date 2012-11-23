SET(PROJECT_NAME Game)
SET(BIN_DIR bin)
SET(DOC_DIR "${BIN_DIR}/doc")
SET(SRC_DIR src)
SET(INCLUDE_DIR .)
SET(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS -std=c++11)

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.6)
#set(CMAKE_CXX_FLAGS "-Wall -W -Werror -ansi -pedantic -g")

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CAL3D REQUIRED)
find_package(SFML COMPONENTS system window network audio graphics REQUIRED)


# Set all debug and info messages on
add_definitions(-DDEBUG=1 -DINFO=1)

### DEFINE NEEDED SOURCES
SET(SOURCES
${SRC_DIR}/main.cpp
${SRC_DIR}/viewer.cpp
${SRC_DIR}/tick.cpp
${SRC_DIR}/TerrainPage.cpp
${SRC_DIR}/Terrain.cpp
${SRC_DIR}/RessourcesManager.cpp
${SRC_DIR}/Textures.cpp
${SRC_DIR}/Texture.cpp
${SRC_DIR}/Shader.cpp
${SRC_DIR}/Node.cpp
${SRC_DIR}/Camera.cpp
${SRC_DIR}/GrassEntity.cpp
${SRC_DIR}/AxisEntity.cpp
${SRC_DIR}/FlagEntity.cpp
${SRC_DIR}/EnvironmentSettings.cpp
)

include_directories (${SRC_DIR} ${OPENGL_INCLUDE_DIR} ${CAL3D_INCLUDE_DIR} ${INCLUDE_DIR})

#### CREATE EXECUTABLE
add_executable (
   ${BIN_DIR}/${PROJECT_NAME} ${SOURCES}
)

target_link_libraries(${BIN_DIR}/${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${CAL3D_LIBRARY} ${SFML_LIBRARIES})




# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
     configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${BIN_DIR}/Doxyfile @ONLY)
     add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${BIN_DIR}/Doxyfile
     WORKING_DIRECTORY ${DOC_DIR}
     COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

