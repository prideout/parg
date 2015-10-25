# parg

[![Build Status](https://travis-ci.org/prideout/parg.svg?branch=master)](https://travis-ci.org/prideout/parg)

This is a C99 library with some basic stuff for bootstrapping a graphics engine.  Currently it is tested against OpenGL 2.1 on OS X, and WebGL 1.0 via Emscripten.

The [single-header API](https://github.com/prideout/parg/blob/master/include/par.h) is divided into the following modules, some of which are implemented with standalone C files that have no dependencies.  These are highlighted in blue.

- **[bluenoise](https://github.com/prideout/parg/blob/master/src/bluenoise.c)** progressive point samples using Recursive Wang Tiles.
- **[filecache](https://github.com/prideout/parg/blob/master/src/filecache.c)** LRU caching on your device's filesystem.
- **[easycurl](https://github.com/prideout/parg/blob/master/src/easycurl.c)** simple HTTP requests; wraps libcurl.
- **token** string-to-uint32 hashing, and a lookup table for uint32-to-string.
- **asset** unified way of loading buffers, shaders, and textures.
- **buffer** an untyped blob of memory that can live on the CPU or GPU.
- **mesh** triangle meshes and utilities for procedural geometry.
- **texture** thin wrapper around OpenGL texture objects.
- **uniform** thin wrapper around OpenGL shader uniforms.
- **state** thin wrapper around miscellaneous portions of the OpenGL state machine.
- **varray** an association of buffers with vertex attributes.
- **draw** thin wrapper around OpenGL draw calls.
- **zcam** simple map-style camera with basic zoom & pan controls.

## How to Build (OS X)

```
brew update
brew tap homebrew/versions
brew install cmake uncrustify glfw3 clang-format pkg-config emscripten
python emsetup.py
. setup.sh
init && build
```

I'm currently using:
- scons 2.3.5
- uncrustify 0.61
- glfw3 3.0.4
- clang-format 3.7.0
- emscripten 1.34.6

## TODO

- offload `par_buffer` to be external in emscripten builds
    - remove ALLOW_MEMORY_GROWTH
- demos/picking.c
    - add rong & tan
- par_data_type should be a normal enum (0 1 2 3...)
    - the mapping to GL types should be internal
- demos/gamma.c
    - see the notes at the top
- demos/turntable
    - add klein to mesh.c
    - see img/wallpaper.png
- gles3 branch: core profile + webgl2 (USE_WEBGL2 exists in emscripten)
- fluid
    - http://codepen.io/tmrDevelops/pen/jbbeMo
- lighting with sdf's
    - https://t.co/sFfcR0hWDo
