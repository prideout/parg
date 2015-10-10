cmake_minimum_required(VERSION 2.8.8)
project(parg)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_library(OPENGL_LIB OpenGL)

file(GLOB DEMOC demos/*.c)
file(GLOB COREC src/*.c)
file(GLOB VENDORC src/vendor/*.c)

include_directories(
    include
    src/vendor
    ${GLFW_INCLUDE_DIRS}
)

add_library(parg STATIC ${COREC} ${VENDORC})

add_executable(
    marina
    demos/marina.c
    marina.glsl
)

add_custom_command(
    OUTPUT marina.glsl
    COMMAND cp ../demos/marina.glsl marina.glsl
    DEPENDS demos/marina.glsl
)

target_link_libraries(
    marina
    parg
    ${OPENGL_LIB}
    ${GLFW_LIBRARIES}
)
