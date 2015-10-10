#include <par.h>
#include "internal.h"
#include "kvec.h"
#include "khash.h"

// Mapping from asset ids (which are tokens) to buffer pointers.
KHASH_MAP_INIT_INT(assmap, par_buffer*)

static khash_t(assmap)* _asset_registry = 0;

static sds _exedir = 0;
static sds _baseurl = 0;

#ifdef EMSCRIPTEN
void par_asset_onload(const char* name, par_buffer* buf)
{
    par_token id = par_token_from_string(name);
    par_verify(buf, "Unable to load asset", 0);
    if (!_asset_registry) {
        _asset_registry = kh_init(assmap);
    }
    int ret;
    int iter = kh_put(assmap, _asset_registry, id, &ret);
    kh_value(_asset_registry, iter) = buf;
}

#else
void par_asset_preload(par_token id)
{
    sds filename = par_token_to_sds(id);
    par_buffer* buf = par_buffer_from_path(filename);
    par_verify(buf, "Unable to load asset", 0);
    if (!_asset_registry) {
        _asset_registry = kh_init(assmap);
    }
    int ret;
    int iter = kh_put(assmap, _asset_registry, id, &ret);
    kh_value(_asset_registry, iter) = buf;
}

#endif

par_buffer* par_asset_to_buffer(par_token id)
{
    par_verify(_asset_registry, "Uninitialized asset registry", 0);
    khiter_t iter = kh_get(assmap, _asset_registry, id);
    par_verify(iter != kh_end(_asset_registry), "Unknown token", 0);
    return kh_value(_asset_registry, iter);
}

sds par_asset_baseurl()
{
    if (!_baseurl) {
        _baseurl = sdsnew("http://github.prideout.net/assets/");
    }
    return _baseurl;
}

#if EMSCRIPTEN

void par_asset_set_baseurl(const char* url) { _baseurl = sdsnew(url); }

sds par_asset_whereami()
{
    if (!_exedir) {
        _exedir = sdsnew("web/");
    }
    return _exedir;
}

int par_asset_fileexists(sds fullpath) { return 1; }

int par_asset_download(const char* filename, sds targetpath) { return 0; }

#else

#include "whereami.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void* kopen(const char* fn, int* _fd);
int kclose(void* a);

sds par_asset_whereami()
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

int par_asset_fileexists(sds fullpath) { return access(fullpath, F_OK) != -1; }

int par_asset_download(const char* filename, sds targetpath)
{
    sds baseurl = par_asset_baseurl();
    sds fullurl = sdscat(sdsdup(baseurl), filename);
    printf("Downloading %s...\n", fullurl);
    int source = 0;
    void* kdfile = kopen(fullurl, &source);
    if (!source) {
        return 0;
    }
    int dest = open(targetpath, O_WRONLY | O_CREAT, 0644);
    char buf[BUFSIZ];
    size_t size;
    size_t total = 0;
    size_t elapsed = 0;
    while ((size = read(source, buf, BUFSIZ)) > 0) {
        if (size > BUFSIZ) {
            close(dest);
            kclose(kdfile);
            remove(targetpath);
            return 0;
        }
        write(dest, buf, size);
        total += size;
        elapsed += size;
        if (elapsed > 1024 * 1024) {
            printf("\t%zu bytes so far...\n", total);
            elapsed = 0;
        }
    }
    printf("\t%zu bytes total.\n", total);
    close(dest);
    kclose(kdfile);
    return 0;
}

#endif
