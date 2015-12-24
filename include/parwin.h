#pragma once

typedef enum {
    PARG_EVENT_DOWN,
    PARG_EVENT_UP,
    PARG_EVENT_MOVE,
    PARG_EVENT_KEYPRESS
} parg_event;

typedef void (*parg_window_fn_init)(float, float, float);
typedef int (*parg_window_fn_tick)(float, float, float, float);
typedef void (*parg_window_fn_input)(parg_event, float, float, float);
typedef void (*parg_window_fn_draw)();
typedef void (*parg_window_fn_exit)(void);
typedef void (*parg_window_fn_message)(const char*);

void parg_window_setargs(int argc, char *argv[]);
void parg_window_oninit(parg_window_fn_init);
void parg_window_ontick(parg_window_fn_tick);
void parg_window_ondraw(parg_window_fn_draw);
void parg_window_onexit(parg_window_fn_exit);
void parg_window_oninput(parg_window_fn_input);
void parg_window_onmessage(parg_window_fn_message);
int parg_window_exec(float winwidth, float winheight, int vsync, int aa);
