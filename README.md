# par

[![Build Status](https://travis-ci.org/prideout/par.svg?branch=master)](https://travis-ci.org/prideout/par)

For now, the `par` library only supports OpenGL 2.1 with OS X, and OpenGL ES 2.0 with Emscripten.

## OS X Setup

```
brew install scons uncrustify glfw3 clang-format
scons --help
```

I'm currently using:
- scons 2.3.5
- uncrustify 0.61
- glfw3 3.0.4
- clang-format 3.7.0
- emscripten 1.33.0

## Notes to Self

- window.c and `par_window_*` api
- tokens.c and khash.c
- shader.c
- simple should draw a triangle

```
include
    par.h
    pargl.h
src
    pargl.c
    token.c
    surface.c
    mesh.c
    buffer.c
    shader.c
    sds.h, sds.c -- only include these if you NEED them for _shaders_, and klib doesn't suffice
    vmath.h (from recipes)
    khash.h (for tokens)
    stb_image.h
demosgl
    gamma.c
    gamma.glsl
    turntable.c
    turntable.glsl
    coordfield.c
    coordfield.glsl
```
