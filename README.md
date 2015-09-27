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

- demos/picking.c
    - factor with clipping.c into mesh.c
    - http://github.prideout.net/distance-picking/
    - add trefoil to mesh.c
    - don't implement rong & tan; cut and run

### Later

- demos/picking.c
    - add rong & tan to picking.c
- demos/gamma.c
    - see the notes at the top
- demos/turntable
    - add klein to mesh.c
    - see img/wallpaper.png
    - pnglite
- window.c
    - add auto-quit and screenshot flags
    - does travis have a GPU?
- emscripten (which should be enabled in travis) and gh-pages
    - don't worry about "offloading" par_buffer yet
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
