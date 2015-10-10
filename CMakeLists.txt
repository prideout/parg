cmake_minimum_required(VERSION 2.8.8)
project(parg)

set(DEMOS
    clipping
    gamma
    marina
    picking
    sierpinski
    simple
    zooming
)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_library(OPENGL_LIB OpenGL)

file(GLOB COREC src/*.c)
file(GLOB VENDORC src/vendor/*.c)

include_directories(
    include
    src/vendor
    ${GLFW_INCLUDE_DIRS}
)

add_library(parg STATIC ${COREC} ${VENDORC})

foreach(DEMONAME ${DEMOS})
    add_executable(
        ${DEMONAME}
        demos/${DEMONAME}.c
        ${DEMONAME}.glsl
    )
    add_custom_command(
        OUTPUT ${DEMONAME}.glsl
        COMMAND cp ../demos/${DEMONAME}.glsl ${DEMONAME}.glsl
        DEPENDS demos/${DEMONAME}.glsl
    )
    target_link_libraries(
        ${DEMONAME}
        parg
        ${OPENGL_LIB}
        ${GLFW_LIBRARIES}
    )
endforeach()
