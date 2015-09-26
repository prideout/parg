#include <par.h>
#include "whereami.h"
#include "sds.h"

static sds _exedir = 0;

const char* par_asset_whereami()
{
    if (!_exedir) {
        int length = wai_getExecutablePath(0, 0, 0);
        _exedir = sdsnewlen("", length);
        int dirlen;
        wai_getExecutablePath(_exedir, length, &dirlen);
        sdsrange(_exedir, 0, dirlen);
    }
    return (const char*) _exedir;
}
