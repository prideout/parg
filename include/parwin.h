#pragma once

typedef void (*par_window_fn_init)(float, float, float);
typedef void (*par_window_fn_tick)(float, float, float, float);
typedef int (*par_window_fn_draw)();
typedef void (*par_window_fn_exit)(void);

void par_window_setargs(int argc, char *argv[]);
void par_window_oninit(par_window_fn_init);
void par_window_ontick(par_window_fn_tick);
void par_window_ondraw(par_window_fn_draw);
void par_window_onexit(par_window_fn_exit);
int par_window_exec(float winwidth, float winheight, int vsync);
