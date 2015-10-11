# par

[![Build Status](https://travis-ci.org/prideout/parg.svg?branch=master)](https://travis-ci.org/prideout/parg)

For now, the `par` library only supports OpenGL 2.1 with OS X, and OpenGL ES 2.0 with Emscripten.

## OS X Setup

```
brew update
brew tap homebrew/versions
brew install cmake uncrustify glfw3 clang-format pkg-config emscripten
python emsetup.py
. env.sh
init && build
```

I'm currently using:
- scons 2.3.5
- uncrustify 0.61
- glfw3 3.0.4
- clang-format 3.7.0
- emscripten 1.34.6

## TODO

- offload the buffer in emscripten builds
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
    - pnglite
- gles3 branch: core profile + webgl2 (USE_WEBGL2 exists in emscripten)
- fluid
    - http://codepen.io/tmrDevelops/pen/jbbeMo
- lighting with sdf's
    - https://t.co/sFfcR0hWDo
