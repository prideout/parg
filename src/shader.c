#include <par.h>
#include "internal.h"
#include "pargl.h"
#include <string.h>
#include "kvec.h"
#include "khash.h"

// Mapping from tokens to sds strings.
KHASH_MAP_INIT_INT(smap, sds)

// Mapping from tokens to OpenGL handles.
KHASH_MAP_INIT_INT(glmap, GLuint)

// Mapping from tokens to integer slots.
KHASH_MAP_INIT_INT(imap, int)

static khash_t(smap)* _vshader_registry = 0;
static khash_t(smap)* _fshader_registry = 0;
static khash_t(glmap)* _program_registry = 0;
static khash_t(imap)* _attr_registry = 0;
static khash_t(imap)* _unif_registry = 0;
static GLuint _current_program = 0;
static par_token _current_program_token = 0;

#define MAX_SHADER_SPEW 1024
#define MAX_UNIFORM_LEN 128
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
    const sds ATTRIBUTE = sdsnew("attribute ");
    const sds PROGRAM = sdsnew("@program ");
    const sds PREFIX = sdsnew("_prefix");

    if (!_vshader_registry) {
        _vshader_registry = kh_init(smap);
        _fshader_registry = kh_init(smap);
        _attr_registry = kh_init(imap);
        _unif_registry = kh_init(imap);
    }

    sdsvec program_args;
    kv_init(program_args);
    sdsvec chunk_bodies;
    kv_init(chunk_bodies);
    sdsvec chunk_names;
    kv_init(chunk_names);

    // Split the buffer into a list of codelines.
    int len = par_buffer_length(buf);
    int nlines;
    char* contents = par_buffer_lock(buf, PAR_READ);
    sds* lines = sdssplitlen(contents, len, "\n", 1, &nlines);
    par_buffer_unlock(buf);

    // Group codelines into chunks and stash all @program lines.
    kv_push(sds, chunk_names, sdsdup(PREFIX));
    kv_push(sds, chunk_bodies, sdsempty());
    for (int j = 0; j < nlines; j++) {
        sds line = lines[j];
        if (sdslen(line) > 2 && line[0] == '-' && line[1] == '-') {
            sds chunkname = sdsdup(line);
            sdsrange(chunkname, 2, -1);
            chunkname = sdstrim(chunkname, " \t");
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
        char* attr = strstr(line, ATTRIBUTE);
        if (attr == line) {
            int nwords;
            sds aline = sdsnew(attr + sdslen(ATTRIBUTE));
            aline = sdstrim(aline, "; \t");
            sds* words = sdssplitlen(aline, sdslen(aline), " \t", 1, &nwords);
            sds attr = sdsdup(words[nwords - 1]);
            sdsfreesplitres(words, nwords);
            sdsfree(aline);
            par_token tok = par_token_from_string(attr);
            khiter_t iter = kh_get(imap, _attr_registry, tok);
            if (iter == kh_end(_attr_registry)) {
                int newslot = kh_size(_attr_registry);
                int ret;
                iter = kh_put(imap, _attr_registry, tok, &ret);
                kh_value(_attr_registry, iter) = newslot;
            }
            sdsfree(attr);
        }
    }
    sdsfreesplitres(lines, nlines);
    sds prefix_body = kv_A(chunk_bodies, 0);

    // Go back through the @program lines and populate the registry.
    for (int p = 0; p < kv_size(program_args); p++) {
        sds argstring = kv_A(program_args, p);

        // Extract the three command arguments.
        int nargs = sdslen(argstring);
        sds* args = sdssplitlen(argstring, nargs, ",", 1, &nargs);
        for (int a = 0; a < nargs; a++) {
            sdstrim(args[a], " \t");
        }
        par_verify(nargs == 3, "@program should have 3 args", 0);

        // Build the vshader and fshader strings.
        int vshader_index = kv_find(chunk_names, args[1]);
        int fshader_index = kv_find(chunk_names, args[2]);
        par_verify(vshader_index > 0, "No such vshader", args[1]);
        par_verify(fshader_index > 0, "No such fshader", args[2]);
        sds vshader_body = kv_A(chunk_bodies, vshader_index);
        sds fshader_body = kv_A(chunk_bodies, fshader_index);
        vshader_body = sdscat(sdsdup(prefix_body), vshader_body);
#if EMSCRIPTEN
        sds qualified_prefix =
            sdscatsds(sdsnew("precision highp float;\n"), prefix_body);
        fshader_body = sdscat(qualified_prefix, fshader_body);
#else
        fshader_body = sdscat(sdsdup(prefix_body), fshader_body);
#endif
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

    sdsfree(ATTRIBUTE);
    sdsfree(PROGRAM);
    sdsfree(PREFIX);
}

void par_shader_load_from_asset(par_token id)
{
    par_buffer* buf = par_buffer_from_asset(id);
    par_shader_load_from_buffer(buf);
    par_buffer_free(buf);
}

GLuint par_shader_attrib(par_token tok) { return 0; }

static GLuint compile_program(par_token tok)
{
    khiter_t iter;

    iter = kh_get(smap, _vshader_registry, tok);
    par_verify(iter != kh_end(_vshader_registry), "No vshader",
        par_token_to_string(tok));
    sds vshader_body = kh_value(_vshader_registry, iter);
    PARGL_STRING vshader_ptr = (PARGL_STRING) &vshader_body;

    iter = kh_get(smap, _fshader_registry, tok);
    par_verify(iter != kh_end(_fshader_registry), "No fshader",
        par_token_to_string(tok));
    sds fshader_body = kh_value(_fshader_registry, iter);
    PARGL_STRING fshader_ptr = (PARGL_STRING) &fshader_body;

    GLchar spew[MAX_SHADER_SPEW];
    GLint compile_success = 0;

    GLuint vs_handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_handle, 1, vshader_ptr, 0);
    glCompileShader(vs_handle);
    glGetShaderiv(vs_handle, GL_COMPILE_STATUS, &compile_success);
    glGetShaderInfoLog(vs_handle, MAX_SHADER_SPEW, 0, spew);
    par_verify(compile_success, par_token_to_string(tok), spew);

    GLuint fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_handle, 1, fshader_ptr, 0);
    glCompileShader(fs_handle);
    glGetShaderiv(fs_handle, GL_COMPILE_STATUS, &compile_success);
    glGetShaderInfoLog(fs_handle, MAX_SHADER_SPEW, 0, spew);
    par_verify(compile_success, par_token_to_string(tok), spew);

    GLuint program_handle = glCreateProgram();
    glAttachShader(program_handle, vs_handle);
    glAttachShader(program_handle, fs_handle);

    for (iter = kh_begin(_attr_registry); iter != kh_end(_attr_registry);
        ++iter) {
        if (!kh_exist(_attr_registry, iter)) {
            continue;
        }
        int slot = kh_value(_attr_registry, iter);
        par_token tok = kh_key(_attr_registry, iter);
        const char* name = par_token_to_string(tok);
        glBindAttribLocation(program_handle, slot, name);
    }

    GLint link_success;
    glLinkProgram(program_handle);
    glGetProgramiv(program_handle, GL_LINK_STATUS, &link_success);
    glGetProgramInfoLog(program_handle, MAX_SHADER_SPEW, 0, spew);
    par_verify(link_success, par_token_to_string(tok), spew);

    return program_handle;
}

static void gather_uniforms(par_token ptoken, GLuint phandle)
{
    int nuniforms;
    glGetProgramiv(phandle, GL_ACTIVE_UNIFORMS, &nuniforms);
    char uname[MAX_UNIFORM_LEN];
    while (nuniforms--) {
        GLint size;
        GLenum type;
        int ret;
        glGetActiveUniform(
            phandle, nuniforms, MAX_UNIFORM_LEN, 0, &size, &type, uname);
        GLint loc = glGetUniformLocation(phandle, uname);
        par_token utoken = par_token_from_string(uname);
        par_token combined_token = ptoken ^ utoken;
        khiter_t iter = kh_put(imap, _unif_registry, combined_token, &ret);
        kh_value(_unif_registry, iter) = loc;
    }
}

GLuint par_shader_attrib_get(par_token tok)
{
    khiter_t iter = kh_get(imap, _attr_registry, tok);
    par_verify(iter != kh_end(_attr_registry), "Unknown attribute",
        par_token_to_string(tok));
    return kh_value(_attr_registry, iter);
}

GLint par_shader_uniform_get(par_token utoken)
{
    par_token ptoken = _current_program_token;
    par_token combined_token = ptoken ^ utoken;
    khiter_t iter = kh_get(imap, _unif_registry, combined_token);
    par_verify(iter != kh_end(_unif_registry), "Inactive uniform",
        par_token_to_string(utoken));
    return kh_value(_unif_registry, iter);
}

void par_shader_bind(par_token tok)
{
    if (!_program_registry) {
        _program_registry = kh_init(glmap);
    }
    khiter_t iter = kh_get(glmap, _program_registry, tok);
    GLuint program = 0;
    int ret;
    if (iter == kh_end(_program_registry)) {
        program = compile_program(tok);
        iter = kh_put(glmap, _program_registry, tok, &ret);
        kh_value(_program_registry, iter) = program;
        gather_uniforms(tok, program);
    } else {
        program = kh_value(_program_registry, iter);
    }
    par_verify(program, "No program", par_token_to_string(tok));
    glUseProgram(program);
    _current_program = program;
    _current_program_token = tok;
}

void par_shader_free(par_token tok)
{
    khiter_t iter = kh_get(glmap, _program_registry, tok);
    par_verify(iter != kh_end(_program_registry), "Unknown program",
        par_token_to_string(tok));
    GLuint program = kh_value(_program_registry, iter);
    glDeleteProgram(program);
    kh_del(glmap, _program_registry, iter);
}
