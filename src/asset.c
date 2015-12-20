#include <parg.h>
#include "internal.h"
#include "kvec.h"
#include "khash.h"
#include "lodepng.h"

// Mapping from asset ids (which are tokens) to buffer pointers.
KHASH_MAP_INIT_INT(assmap, parg_buffer*)

static khash_t(assmap)* _asset_registry = 0;

static sds _exedir = 0;
static sds _baseurl = 0;

#ifdef EMSCRIPTEN
void parg_asset_onload(const char* name, parg_buffer* buf)
{
    parg_token id = parg_token_from_string(name);
    parg_assert(buf, "Unable to load asset");
    if (!_asset_registry) {
        _asset_registry = kh_init(assmap);
    }
    int ret;
    int iter = kh_put(assmap, _asset_registry, id, &ret);
    kh_value(_asset_registry, iter) = buf;
}

#else

static sds _pngsuffix = 0;

void parg_asset_preload(parg_token id)
{
    if (!_pngsuffix) {
        _pngsuffix = sdsnew(".png");
    }
    sds filename = parg_token_to_sds(id);
    parg_buffer* buf = parg_buffer_from_path(filename);
    parg_assert(buf, "Unable to load asset");
    if (sdslen(filename) > 4) {
        sds suffix = sdsdup(filename);
        sdsrange(suffix, -4, -1);
        if (!sdscmp(suffix, _pngsuffix)) {
            unsigned char* decoded;
            unsigned dims[3] = {0, 0, 4};
            unsigned char* filedata = parg_buffer_lock(buf, PARG_READ);
            unsigned err = lodepng_decode_memory(&decoded, &dims[0], &dims[1],
                    filedata, parg_buffer_length(buf), LCT_RGBA, 8);
            parg_assert(err == 0, "PNG decoding error");
            parg_buffer_free(buf);
            int nbytes = dims[0] * dims[1] * dims[2];
            buf = parg_buffer_alloc(nbytes + 12, PARG_CPU);
            int* ptr = parg_buffer_lock(buf, PARG_WRITE);
            *ptr++ = dims[0];
            *ptr++ = dims[1];
            *ptr++ = dims[2];
            memcpy(ptr, decoded, nbytes);
            free(decoded);
            parg_buffer_unlock(buf);
        }
        sdsfree(suffix);
    }
    if (!_asset_registry) {
        _asset_registry = kh_init(assmap);
    }
    int ret;
    int iter = kh_put(assmap, _asset_registry, id, &ret);
    kh_value(_asset_registry, iter) = buf;
}

#endif

parg_buffer* parg_asset_to_buffer(parg_token id)
{
    parg_assert(_asset_registry, "Uninitialized asset registry");
    khiter_t iter = kh_get(assmap, _asset_registry, id);
    parg_assert(iter != kh_end(_asset_registry), "Unknown token");
    return kh_value(_asset_registry, iter);
}

sds parg_asset_baseurl()
{
    if (!_baseurl) {
        _baseurl = sdsnew("http://github.prideout.net/assets/");
    }
    return _baseurl;
}

#if EMSCRIPTEN

void parg_asset_set_baseurl(const char* url) { _baseurl = sdsnew(url); }

sds parg_asset_whereami()
{
    if (!_exedir) {
        _exedir = sdsnew("web/");
    }
    return _exedir;
}

int parg_asset_fileexists(sds fullpath) { return 1; }

int parg_asset_download(const char* filename, sds targetpath) { return 0; }

#else

#include "whereami.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define PAR_EASYCURL_IMPLEMENTATION
#include <par/par_easycurl.h>

sds parg_asset_whereami()
{
    if (!_exedir) {
        int length = wai_getExecutablePath(0, 0, 0);
        _exedir = sdsnewlen("", length);
        int dirlen;
        wai_getExecutablePath(_exedir, length, &dirlen);
        sdsrange(_exedir, 0, dirlen);
    }
    return _exedir;
}

int parg_asset_fileexists(sds fullpath) { return access(fullpath, F_OK) != -1; }

int parg_asset_download(const char* filename, sds targetpath)
{
    sds baseurl = parg_asset_baseurl();
    sds fullurl = sdscat(sdsdup(baseurl), filename);
    printf("Downloading %s...\n", fullurl);
    return par_easycurl_to_file(fullurl, targetpath);
    return 0;
}

#endif
