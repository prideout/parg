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

#include <curl/curl.h>
#include <strings.h>
#include <stdlib.h>
#include "par.h"

static int _ready = 0;

void par_easycurl_init(uint32_t flags)
{
    if (!_ready) {
        curl_global_init(CURL_GLOBAL_SSL);
        _ready = 1;
    }
}

void par_easycurl_shutdown()
{
    if (_ready) {
        curl_global_cleanup();
    }
}

static size_t onheader(void* v, size_t size, size_t nmemb)
{
    size_t n = size * nmemb;
    char* h = v;
    if (n > 14 && !strncasecmp("Last-Modified:", h, 14)) {
        const char* s = h + 14;
        time_t r = curl_getdate(s, 0);
        if (r != -1) {
            // TODO handle last-modified
        }
    } else if (n > 5 && !strncasecmp("ETag:", h, 5)) {
        // TODO handle etag
    }
    return n;
}

typedef struct {
    par_byte* data;
    int nbytes;
} par_easycurl_buffer;

static size_t onwrite(char* contents, size_t size, size_t nmemb, void* udata)
{
    size_t realsize = size * nmemb;
    par_easycurl_buffer* mem = (par_easycurl_buffer*) udata;
    mem->data = realloc(mem->data, mem->nbytes + realsize + 1);
    if (!mem->data) {
        return 0;
    }
    memcpy(mem->data + mem->nbytes, contents, realsize);
    mem->nbytes += realsize;
    mem->data[mem->nbytes] = 0;
    return realsize;
}

int par_easycurl_to_memory(const char* url, par_byte** data, int* nbytes)
{
    par_easycurl_buffer buffer = {malloc(1), 0};
    long code = 0;
    long status = 0;
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(handle, CURLOPT_ENCODING, "gzip, deflate");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 8);
    curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, onwrite);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, onheader);
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
    curl_easy_setopt(handle, CURLOPT_TIMEVALUE, 0);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, 0);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 15);
    curl_easy_perform(handle);
    curl_easy_getinfo(handle, CURLINFO_CONDITION_UNMET, &code);
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status);
    if (status == 304 || status >= 400) {
        return 0;
    }
    *data = buffer.data;
    *nbytes = buffer.nbytes;
    curl_easy_cleanup(handle);
    return 1;
}

int par_easycurl_to_file(const char* srcurl, const char* dstpath)
{
    par_easycurl_buffer buffer = {malloc(1), 0};
    long code = 0;
    long status = 0;
    FILE* filehandle = fopen(dstpath, "wb");
    if (!filehandle) {
        return 0;
    }
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(handle, CURLOPT_ENCODING, "gzip, deflate");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 8);
    curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, filehandle);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, onheader);
    curl_easy_setopt(handle, CURLOPT_URL, srcurl);
    curl_easy_setopt(handle, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
    curl_easy_setopt(handle, CURLOPT_TIMEVALUE, 0);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, 0);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 15);
    curl_easy_perform(handle);
    curl_easy_getinfo(handle, CURLINFO_CONDITION_UNMET, &code);
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status);
    fclose(filehandle);
    if (status == 304 || status >= 400) {
        remove(dstpath);
        return 0;
    }
    curl_easy_cleanup(handle);
    return 1;
}
