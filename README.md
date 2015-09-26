# par

[![Build Status](https://travis-ci.org/prideout/par.svg?branch=master)](https://travis-ci.org/prideout/par)

For now, the `par` library only supports OpenGL 2.1 with OS X, and OpenGL ES 2.0 with Emscripten.

## OS X Setup

```
brew update
brew install scons uncrustify glfw3 clang-format
scons
```

I'm currently using:
- scons 2.3.5
- uncrustify 0.61
- glfw3 3.0.4
- clang-format 3.7.0
- emscripten 1.33.0

## TODO

### Soon

- simple should draw a triangle
- gamma
    - surface.c (sphere, klein, disk)
    - mesh.c
    - https://www.shadertoy.com/view/llBGz1

### Later

- emscripten (which should be enabled in travis) and gh-pages
    - don't worry about "offloading" par_buffer yet
- turntable
- coordfield (picking)
- gles3 branch: core profile + webgl2 (USE_WEBGL2 exists in emscripten)
- fluid
    - http://codepen.io/tmrDevelops/pen/jbbeMo
- lighting with sdf's
    - https://t.co/sFfcR0hWDo
