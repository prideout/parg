# parg

[![Build Status](https://travis-ci.org/prideout/parg.svg?branch=master)](https://travis-ci.org/prideout/parg)

This is a C99 library with some basic stuff for bootstrapping a graphics engine.  Currently it is tested against OpenGL 2.1 on OS X, and WebGL 1.0 via Emscripten.

The [par](https://github.com/prideout/par) library is a required submodule, so be sure to do this after cloning the repo:

```bash
$ git submodule update --init
```

The [API](https://github.com/prideout/parg/blob/master/include/parg.h) is divided into modules, each of which has a single C file:

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

## How to Build for macOS

```bash
brew update
brew install cmake uncrustify glfw clang-format pkg-config
mkdir build ; cd build ; cmake .. ; make -j
```

You can then run a demo like this:
```bash
./trefoil
```


## How to Build for WebGL on macOS

```bash
brew update
brew install cmake uncrustify glfw clang-format pkg-config emscripten
python emsetup.py
source aliases.sh
initjs && build
```

You can then run a demo like this:
```bash
build/trefoil
[Push Esc to quit]
cd web
python -m SimpleHTTPServer
[Open localhost:8000 in a web browser]
```


I'm currently using:
- glfw 3.3
- emscripten 1.36.5
