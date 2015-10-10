#include "asset.h"
#include <par.h>
#include <assert.h>

static sds _exedir = 0;
static sds _baseurl = 0;

sds par_token_to_sds(par_token token);

void par_asset_preload(par_token id)
{
    sds filename = par_token_to_sds(id);
    // ...
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
