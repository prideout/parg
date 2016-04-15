cmake_minimum_required(VERSION 2.8.8)
project(parg)

option(EMSCRIPTEN "enable JavaScript output via emcc")

set(DEMOS
    dragpts
    bubbles
    shapes
    clipping
    klein
    marina
    trefoil
    sierpinski
    simple
    zooming
    trillium
    terrainpts
    hmap
    ztex
    orbits)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(CURL REQUIRED libcurl)
find_library(OPENGL_LIB OpenGL)

file(GLOB COREC src/*.c)
file(GLOB VENDORC extern/*.c)
file(GLOB SRCFILES ${COREC} ${VENDORC})
file(GLOB JSEXCLUSIONS src/window.c src/easycurl.c src/filecache.c)
file(GLOB JSCPP src/bindings.cpp src/objloader.cpp)

include_directories(
    include
    extern
    ${GLFW_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-gnu99")
    find_package (X11 REQUIRED)
    add_definitions(
        -O3
        -Wall
        -I../include
        -I../extern)
    set(PLATFORM_LIBS
        ${X11_LIBRARIES}
        Xinerama
        pthread
        dl
        Xrandr
        Xinerama
        Xcursor
        Xi
        Xxf86vm
        GL
        m)
endif()

add_library(parg STATIC ${SRCFILES} src/objloader.cpp)

set(EMCCARGS
    -c
    -std=c99
    -O3
    -Wall
    -I../include
    -I../extern)
set(EMCXXARGS
    -O3
    -std=c++11
    -Wall
    -I../include
    -I../extern)
set(EMLINKARGS
    -O3 --memory-init-file 0 --bind
    --post-js ../web/js/suffix.js
    -s 'ALLOW_MEMORY_GROWTH=1'
    -s 'MODULARIZE=1'
    -s 'EXPORT_NAME="CreateParg"'
    -s 'NO_FILESYSTEM=1'
    -s 'PRECISE_F32=1')

set(JSSRC ${SRCFILES})
list(REMOVE_ITEM JSSRC ${JSEXCLUSIONS})
foreach(SRCFILE ${JSSRC})
    get_filename_component(BASENAME ${SRCFILE} NAME_WE)
    set(JSOBJECT ${BASENAME}.js.o)
    add_custom_command(
        OUTPUT ${JSOBJECT}
        COMMAND emcc -o ${JSOBJECT} ${EMCCARGS} ${SRCFILE}
        DEPENDS ${SRCFILE}
    )
    list(APPEND JSOBJECTS ${JSOBJECT})
endforeach()
foreach(SRCFILE ${JSCPP})
    get_filename_component(BASENAME ${SRCFILE} NAME_WE)
    set(JSOBJECT ${BASENAME}.js.o)
    add_custom_command(
        OUTPUT ${JSOBJECT}
        COMMAND emcc -o ${JSOBJECT} ${EMCXXARGS} ${SRCFILE}
        DEPENDS ${SRCFILE}
    )
    list(APPEND JSOBJECTS ${JSOBJECT})
endforeach()

foreach(DEMONAME ${DEMOS})
    if(EMSCRIPTEN)
        add_executable(
            ${DEMONAME}
            demos/${DEMONAME}.c
            ${DEMONAME}.glsl
            ${DEMONAME}.js)
    else()
        add_executable(
            ${DEMONAME}
            demos/${DEMONAME}.c
            ${DEMONAME}.glsl)
    endif()
    add_custom_command(
        OUTPUT ${DEMONAME}.glsl
        COMMAND cp ../demos/${DEMONAME}.glsl ${DEMONAME}.glsl
        DEPENDS demos/${DEMONAME}.glsl)
    target_link_libraries(
        ${DEMONAME}
        parg
        ${OPENGL_LIB}
        ${GLFW_LIBRARIES}
        ${CURL_LIBRARIES}
        ${PLATFORM_LIBS})
    add_custom_command(
        OUTPUT ${DEMONAME}.js.o
        COMMAND emcc -o ${DEMONAME}.js.o ${EMCCARGS} ../demos/${DEMONAME}.c
        DEPENDS demos/${DEMONAME}.c)
    add_custom_command(
        OUTPUT ${DEMONAME}.js
        COMMAND emcc -o ${DEMONAME}.js ${EMLINKARGS} ${JSOBJECTS} ${DEMONAME}.js.o
        DEPENDS ${DEMONAME}.js.o ${JSOBJECTS} ${POSTJS})
endforeach()
