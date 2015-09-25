#include <par.h>
#include <stdio.h>
#include "khash.h"

KHASH_SET_INIT_STR(parstrset)
#define par_string_set khash_t(parstrset)

static par_string_set *_token_registry = 0;

void par_token_init()
{
    _token_registry = kh_init(parstrset);
}

void par_token_shutdown()
{
	kh_destroy(parstrset, _token_registry);
}

const char* par_token_to_string(par_token token)
{
    return kh_key(_token_registry, token);
}

par_token par_token_from_string(const char* cstring)
{
    if (!_token_registry) {
        par_token_init();
    }
    int ret;
	return kh_put(parstrset, _token_registry, cstring, &ret);
}
