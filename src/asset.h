#pragma once

#include <sds.h>

sds par_asset_whereami();
void par_asset_set_baseurl(const char* url);
sds par_asset_baseurl();
int par_asset_fileexists(sds fullpath);
int par_asset_download(const char* filename, sds targetpath);
