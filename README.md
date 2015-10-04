# par

[![Build Status](https://travis-ci.org/prideout/parg.svg?branch=master)](https://travis-ci.org/prideout/parg)

For now, the `par` library only supports OpenGL 2.1 with OS X, and OpenGL ES 2.0 with Emscripten.

## OS X Setup

```
brew update
brew tap homebrew/versions
brew install scons uncrustify glfw3 clang-format pkg-config
scons
```

I'm currently using:
- scons 2.3.5
- uncrustify 0.61
- glfw3 3.0.4
- clang-format 3.7.0
- emscripten 1.33.0

## TODO

- demos/picking.c
    - add rong & tan
- emscripten (which should be enabled in travis) and gh-pages
- par_data_type should be a normal enum (0 1 2 3...)
    - the mapping to GL types should be internal
- demos/gamma.c
    - see the notes at the top
- demos/turntable
    - add klein to mesh.c
    - see img/wallpaper.png
    - pnglite
- offload the buffer in emscripten builds
- gles3 branch: core profile + webgl2 (USE_WEBGL2 exists in emscripten)
- fluid
    - http://codepen.io/tmrDevelops/pen/jbbeMo
- lighting with sdf's
    - https://t.co/sFfcR0hWDo

## Snippets

```c
// BUFVIEW

#define PAR_DECLARE_BUFVIEW(name, type)
#define PAR_DEFINE_BUFVIEW(name, type)

PAR_DECLARE_BUFVIEW(par_fp32, float);
PAR_DECLARE_BUFVIEW(par_u16, unsigned short);

typedef struct par_fp32_s par_fp32;
typedef struct par_u16_s par_u16;

par_fp32* par_fp32_wrap(par_buffer*);
par_buffer* par_fp32_get(par_fp32*);

par_fp32* par_fp32_alloc(int nfloats);
void par_fp32_free(par_fp32*);
int par_fp32_length(par_fp32*);
float* par_fp32_lock(par_fp32*, par_buffer_mode);
void par_fp32_unlock(par_fp32*);
```
