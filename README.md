# parg

[![Build Status](https://travis-ci.org/prideout/parg.svg?branch=master)](https://travis-ci.org/prideout/parg)

This is a C99 library with some basic stuff for bootstrapping a graphics engine.  Currently it is tested against OpenGL 2.1 on OS X, and WebGL 1.0 via Emscripten.

The [API](https://github.com/prideout/parg/blob/master/include/par.h) is divided into modules, each of which has a single C file.  Some of the C files have no dependencies on the rest of the library, making them easy to cherry pick into any project.  These standalone C files are:

- **[bluenoise](https://github.com/prideout/parg/blob/master/src/bluenoise.c)** progressive point samples using Recursive Wang Tiles.
- **[filecache](https://github.com/prideout/parg/blob/master/src/filecache.c)** LRU caching on your device's filesystem.
- **[easycurl](https://github.com/prideout/parg/blob/master/src/easycurl.c)** simple HTTP requests; wraps libcurl.
- **[msquares](https://github.com/prideout/parg/blob/master/src/msquares.c)** an efficient marching squares implementation.

The non-standalone modules of the library are listed below.

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
