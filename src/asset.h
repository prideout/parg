#pragma once

#include <sds.h>

sds par_asset_whereami();
int par_asset_fileexists(sds fullpath);
int par_asset_download(const char* filename, sds targetpath);
