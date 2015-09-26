#include <par.h>
#include <sds.h>
#include <stdio.h>
#include <string.h>
#include "verify.h"
#include "kvec.h"
#include "khash.h"

// Singleton mapping from tokens to sds strings.
KHASH_MAP_INIT_INT(smap, sds)

static khash_t(smap)* _vshader_registry = 0;
static khash_t(smap)* _fshader_registry = 0;

#define kv_last(vec) kv_A(vec, kv_size(vec) - 1)

#define chunk_body kv_last(chunk_bodies)

typedef kvec_t (sds) sdsvec;

static int kv_find(sdsvec keys, sds key)
{
    for (int p = 0; p < kv_size(keys); p++) {
        if (0 == strcmp(kv_A(keys, p), key)) {
            return p;
        }
    }
    return -1;
}

void par_shader_load_from_buffer(par_buffer* buf)
{
    const sds PROGRAM = sdsnew("@program ");
    const sds PREFIX = sdsnew("_prefix");

    if (!_vshader_registry) {
        _vshader_registry = kh_init(smap);
        _fshader_registry = kh_init(smap);
    }

    sdsvec program_args;
    kv_init(program_args);

    sdsvec chunk_bodies;
    kv_init(chunk_bodies);

    sdsvec chunk_names;
    kv_init(chunk_names);

    int len = par_buffer_length(buf);
    int nlines;
    char* contents = par_buffer_lock(buf, PAR_READ);
    sds* lines = sdssplitlen(contents, len, "\n", 1, &nlines);
    par_buffer_unlock(buf);

    kv_push(sds, chunk_names, sdsdup(PREFIX));
    kv_push(sds, chunk_bodies, sdsempty());

    // Split the GLSL file into chunks and stash all @program lines.

    for (int j = 0; j < nlines; j++) {
        sds line = lines[j];
        if (sdslen(line) > 2 && line[0] == '-' && line[1] == '-') {
            sds chunkname = sdsdup(line);
            sdsrange(chunkname, 2, -1);
            sdstrim(chunkname, " \t");
            kv_push(sds, chunk_names, chunkname);
            kv_push(sds, chunk_bodies, sdsempty());
            chunk_body = sdscatprintf(chunk_body, "#line %d\n", j + 1);
            continue;
        }
        chunk_body = sdscatsds(chunk_body, line);
        chunk_body = sdscat(chunk_body, "\n");
        char* program = strstr(line, PROGRAM);
        if (program) {
            kv_push(sds, program_args, sdsnew(program + sdslen(PROGRAM)));
        }
    }

    sdsfreesplitres(lines, nlines);
    sds prefix_body = kv_A(chunk_bodies, 0);

    for (int p = 0; p < kv_size(program_args); p++) {

        // Extract @program arguments.

        sds argstring = kv_A(program_args, p);
        int nargs = sdslen(argstring);
        sds* args = sdssplitlen(argstring, nargs, ",", 1, &nargs);
        for (int a = 0; a < nargs; a++) {
            sdstrim(args[a], " \t");
        }
        par_verify(nargs == 3, "@program should have 3 args", 0);

        // Build the vshader and fshader strings.

        int vshader_index = kv_find(chunk_names, args[1]);
        int fshader_index = kv_find(chunk_names, args[2]);

        sds vshader_body = kv_A(chunk_bodies, vshader_index);
        sds fshader_body = kv_A(chunk_bodies, fshader_index);

        vshader_body = sdscat(sdsdup(prefix_body), vshader_body);
        fshader_body = sdscat(sdsdup(prefix_body), fshader_body);

        par_token program_name = par_token_from_string(args[0]);
        sdsfreesplitres(args, nargs);
        sdsfree(argstring);

        // Insert the vshader and fshader strings into the registries.

        int ret;
        khiter_t iter;

        iter = kh_put(smap, _vshader_registry, program_name, &ret);
        kh_value(_vshader_registry, iter) = vshader_body;

        iter = kh_put(smap, _fshader_registry, program_name, &ret);
        kh_value(_fshader_registry, iter) = fshader_body;
    }

    kv_destroy(program_args);
    kv_destroy(chunk_bodies);
    kv_destroy(chunk_names);
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
