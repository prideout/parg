// parg: a tiny C library of GL utilities and demos
// https://github.com/prideout/parg
//
// The MIT License
// Copyright (c) 2015 Philip Rideout
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "par.h"
#include "lz4.h"
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ENTRIES 64

typedef struct {
    time_t last_used_timestamp;
    uint64_t hashed_name;
    const char* name;
    int nbytes;
} filecache_entry_t;

typedef struct {
    filecache_entry_t entries[MAX_ENTRIES];
    int nentries;
    int totalbytes;
} filecache_table_t;

static void _update_table(const char* item_name, int item_size);
static void _append_table(const char* item_name, int item_size);
static void _read_or_create_tablefile();
static void _save_tablefile();
static void _evict_lru();
static uint64_t _hash(const char* name);

static char _fileprefix[PATH_MAX] = "./_cache.";
static char _tablepath[PATH_MAX] = "./_cache.table";
static int _maxtotalbytes = 1024 * 1024 * 16;
static filecache_table_t* _table = 0;

void par_filecache_init(const char* prefix, int maxsize)
{
    size_t len = strlen(prefix);
    assert(len + 1 < PATH_MAX && "Cache prefix is too long");
    strncpy(_fileprefix, prefix, len + 1);
    strcpy(_tablepath, _fileprefix);
    strcat(_tablepath, "table");
    _maxtotalbytes = maxsize;
}

#if IOS_EXAMPLE
NSString* getPrefix()
{
    NSString *cachesFolder = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject];
    NSError *error = nil;
    if (![[NSFileManager defaultManager] createDirectoryAtPath:cachesFolder withIntermediateDirectories:YES attributes:nil error:&error]) {
        NSLog(@"MGMPlatformGetCachesFolder error: %@", error);
        return nil;
    }
    return [cachesFolder stringByAppendingString:@"/_cache."];
}
#endif

int par_filecache_load(const char* name, par_byte** payload, int* payloadsize,
    par_byte* header, int headersize)
{
    char qualified[PATH_MAX];
    size_t len = strlen(name);
    if (len == 0) {
        return 0;
    }
    assert(len + strlen(_fileprefix) < PATH_MAX);
    strcpy(qualified, _fileprefix);
    strcat(qualified, name);
    if (access(qualified, F_OK) == -1) {
        return 0;
    }
    FILE* cachefile = fopen(qualified, "rb");
    assert(cachefile && "Unable to open cache file for reading");
    fseek(cachefile, 0, SEEK_END);
    long fsize = ftell(cachefile);
    fseek(cachefile, 0, SEEK_SET);
    if (headersize > 0) {
        fread(header, headersize, 1, cachefile);
    }
    int32_t dnbytes;
    long cnbytes = fsize - headersize - sizeof(dnbytes);
    fread(&dnbytes, 1, sizeof(dnbytes), cachefile);
    char* cbuff = malloc(cnbytes);
    fread(cbuff, 1, cnbytes, cachefile);
    #if ENABLE_LZ4
    char* dbuff = malloc(dnbytes);
    LZ4_decompress_safe(cbuff, dbuff, (int) cnbytes, dnbytes);
    free(cbuff);
    #else
    char* dbuff = cbuff;
    #endif
    fclose(cachefile);
    *payload = (par_byte*) dbuff;
    *payloadsize = dnbytes;
    _update_table(name, (int) cnbytes);
    return 1;
}

void par_filecache_save(const char* name, par_byte* payload, int payloadsize,
    par_byte* header, int headersize)
{
    char qualified[PATH_MAX];
    size_t len = strlen(name);
    if (len == 0) {
        return;
    }
    assert(len + strlen(_fileprefix) < PATH_MAX);
    strcpy(qualified, _fileprefix);
    strcat(qualified, name);
    FILE* cachefile = fopen(qualified, "wb");
    assert(cachefile && "Unable to open cache file for writing");
    if (headersize > 0) {
        fwrite(header, 1, headersize, cachefile);
    }
    int csize = 0;
    if (payloadsize > 0) {
        int32_t nbytes = payloadsize;
        fwrite(&nbytes, 1, sizeof(nbytes), cachefile);
        #if ENABLE_LZ4
        int maxsize = LZ4_compressBound(nbytes);
        char* dst = malloc(maxsize);
        const char* src = (const char*) payload;
        assert(nbytes < LZ4_MAX_INPUT_SIZE);
        csize = LZ4_compress_default(src, dst, nbytes, maxsize);
        fwrite(dst, 1, csize, cachefile);
        free(dst);
        #else
        csize = payloadsize;
        fwrite(payload, 1, csize, cachefile);
        #endif
    }
    fclose(cachefile);
    _append_table(name, csize + headersize);
}

// Adds the given item to the table and evicts the LRU items if the total cache
// size exceeds the specified maxsize.
static void _append_table(const char* item_name, int item_size)
{
    time_t now = time(0);
    if (!_table) {
        _read_or_create_tablefile();
    }
    int64_t hashed_name = _hash(item_name);
    int total = _table->totalbytes + item_size;
    while (_table->nentries >= MAX_ENTRIES || total > _maxtotalbytes) {
        assert(_table->nentries > 0 && "Cache size is too small.");
        _evict_lru();
        total = _table->totalbytes + item_size;
    }
    _table->totalbytes = total;
    filecache_entry_t* entry = &_table->entries[_table->nentries++];
    entry->last_used_timestamp = now;
    entry->hashed_name = hashed_name;
    entry->name = strdup(item_name);
    entry->nbytes = item_size;
    _save_tablefile();
}

// Updates the timestamp associated with the given item.
static void _update_table(const char* item_name, int item_size)
{
    time_t now = time(0);
    if (!_table) {
        _read_or_create_tablefile();
    }
    int64_t hashed_name = _hash(item_name);
    filecache_entry_t* entry = _table->entries;
    int i;
    for (i = 0; i < _table->nentries; i++, entry++) {
        if (entry->hashed_name == hashed_name) {
            break;
        }
    }
    if (i < _table->nentries) {
        _append_table(item_name, item_size);
        return;
    }
    entry->last_used_timestamp = now;
    _save_tablefile();
}

static void _read_or_create_tablefile()
{
    _table = calloc(sizeof(filecache_table_t), 1);
    FILE* fhandle = fopen(_tablepath, "r");
    if (!fhandle) {
        fhandle = fopen(_tablepath, "w");
        assert(fhandle && "Unable to create filecache info file.");
    } else {
        filecache_entry_t entry;
        char name[PATH_MAX];
        while (1) {
            int nargs = fscanf(fhandle, "%ld %d %s", &entry.last_used_timestamp,
                &entry.nbytes, name);
            if (nargs != 3) {
                break;
            }
            entry.name = strdup(name);
            entry.hashed_name = _hash(entry.name);
            _table->entries[_table->nentries++] = entry;
            _table->totalbytes += entry.nbytes;
        }
    }
    fclose(fhandle);
}

static void _save_tablefile()
{
    FILE* fhandle = fopen(_tablepath, "w");
    assert(fhandle && "Unable to create filecache info file.");
    filecache_entry_t* entry = _table->entries;
    for (int i = 0; i < _table->nentries; i++, entry++) {
        fprintf(fhandle, "%ld %d %s\n", entry->last_used_timestamp,
            entry->nbytes, entry->name);
    }
    fclose(fhandle);
}

static void _evict_lru()
{
    int oldest_index = -1;
    time_t oldest_time = LONG_MAX;
    filecache_entry_t* entry = _table->entries;
    for (int i = 0; i < _table->nentries; i++, entry++) {
        if (entry->last_used_timestamp < oldest_time) {
            oldest_time = entry->last_used_timestamp;
            oldest_index = i;
        }
    }
    if (oldest_index > -1) {
        entry = _table->entries + oldest_index;
        char qualified[PATH_MAX];
        size_t len = strlen(entry->name);
        assert(len + strlen(_fileprefix) < PATH_MAX);
        strcpy(qualified, _fileprefix);
        strcat(qualified, entry->name);
        printf("Evicting %s\n", entry->name);
        remove(qualified);
        _table->totalbytes -= entry->nbytes;
        if (_table->nentries-- > 1) {
            *entry = _table->entries[_table->nentries];
        }
    }
}

// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function

static uint64_t _hash(const char* name)
{
    const uint64_t OFFSET = 14695981039346656037ull;
    const uint64_t PRIME  = 1099511628211ull;
    const unsigned char* str = (const unsigned char*) name;
    uint64_t hval = OFFSET;
    while (*str) {
        hval *= PRIME;
        hval ^= (uint64_t) *(str++);
    }
    return hval;
}
