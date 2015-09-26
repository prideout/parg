#include <par.h>
#include <sds.h>
#include <stdio.h>
#include <string.h>
#include "verify.h"
#include "kvec.h"

void par_shader_load_from_buffer(par_buffer* buf)
{
    const sds PROGRAM = sdsnew("@program ");

    kvec_t(sds) program_args;
    kv_init(program_args);

    kvec_t(sds) chunk_bodies;
    kv_init(chunk_bodies);

    kvec_t(sds) chunk_names;
    kv_init(chunk_names);

    int len = par_buffer_length(buf);
    int nlines;
    char* contents = par_buffer_lock(buf, PAR_READ);
    sds* lines = sdssplitlen(contents, len, "\n", 1, &nlines);
    par_buffer_unlock(buf);
    sds chunk = 0;

    for (int j = 0; j < nlines; j++) {
        sds line = lines[j];
        if (sdslen(line) > 2 && line[0] == '-' && line[1] == '-') {
            sdsrange(line, 2, -1);
            sdstrim(line, " \t");
            sdsfree(chunk);
            chunk = sdsdup(line);
            printf("chunk>> %s\n", line);
            continue;
        }
        printf("line-- %s\n", line);
        char* found = strstr(line, PROGRAM);
        if (found) {
            kv_push(sds, program_args, sdsnew(found + sdslen(PROGRAM)));
        }
    }

    sdsfreesplitres(lines, nlines);
    sdsfree(chunk);

    for (int p = 0; p < kv_size(program_args); p++) {
        sds argstring = kv_A(program_args, p);
        int nargs = sdslen(argstring);
        sds* args = sdssplitlen(argstring, nargs, ",", 1, &nargs);
        for (int a = 0; a < nargs; a++) {
            sdstrim(args[a], " \t");
        }
        par_verify(nargs == 3, "@program should have 3 args", 0);
        sdsfreesplitres(args, nargs);
        sdsfree(argstring);
    }

    kv_destroy(program_args);
    kv_destroy(chunk_bodies);
    kv_destroy(chunk_names);
    printf("TODO: parse shader\n");
}

void par_shader_load_from_asset(const char* filename)
{
    par_buffer* buf = par_buffer_from_asset(filename);
    par_shader_load_from_buffer(buf);
    par_buffer_free(buf);
}

GLuint par_shader_attrib(par_token tok) { return 0; }

GLint par_shader_uniform(par_token tok) { return 0; }

void par_shader_bind(par_token tok) {}
