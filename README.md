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

## How to Build (OS X)

```bash
brew update
brew tap homebrew/versions
brew install cmake uncrustify glfw3 clang-format pkg-config emscripten
python emsetup.py
source aliases.sh
initjs && build
```

You can then run a native demo like this:
```bash
build/trefoil
[Push Esc to quit]
cd web
python -m SimpleHTTPServer
[Open localhost:8000 in a web browser]
```


I'm currently using:
- glfw3 3.1.2
- emscripten 1.36.5

## How to Build for Windows (experimental)

These are the magic ingredients that I use for Windows builds:
- Visual Studio 2015 (it's free!)
- [msys2](http://msys2.github.io/)
- CMake

Open the mingw64.exe prompt and do this:

```bash
pacman -Syu  # to update the system packages (errors are ignoreable)
pacman -S mingw-w64-x86_64-pkg-config
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-glfw
pacman -S git libcurl
```

Next, cd to the git repo and use the "init" alias to invoke CMake:

```bash
source aliases.sh
init
```

At this point you can either use the "build" alias, or you can open the generated project at `build/parg.sln`.

As an aside, take a look at [winpty](https://github.com/rprichard/winpty) if you'd like to use emacs in your msys console.
