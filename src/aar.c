#include <parg.h>
#include <math.h>

float parg_aar_to_tilerange(parg_aar rect, Vector2 size, parg_tilerange* range)
{
    float e = PARG_MAX(size.x, size.y);
    Vector2 mapsquare = {e, e};
    Vector2 mapmin = {e * -.5, e * -.5};
    parg_aar sqrrect = parg_aar_stretch_to_square(rect);
    float mag = size.y / parg_aar_height(sqrrect);
    float z = log(mag) / log(2.0f);
    parg_tilename tilename = {0, 0, z};
    mag = 1 << (int) z;
    float tilesize = mapsquare.x / mag;
    range->mintile = tilename;
    range->maxtile = tilename;
    range->mintile.x = (rect.left - mapmin.x) / tilesize;
    range->mintile.y = (rect.bottom - mapmin.y) / tilesize;
    range->maxtile.x = (rect.right - mapmin.x) / tilesize;
    range->maxtile.y = (rect.top - mapmin.y) / tilesize;
    return fmodf(z, 1.0);
}

parg_aar parg_aar_from_tilename(parg_tilename tile, Vector2 mapsize)
{
    float e = PARG_MAX(mapsize.x, mapsize.y);
    Vector2 mapsquare = {e, e};
    Vector2 mapmin = {e * -.5, e * -.5};
    float mag = 1 << (int) tile.z;
    float tilesize = mapsquare.x / mag;
    parg_aar rect;
    rect.left = mapmin.x + tilesize * tile.x;
    rect.bottom = mapmin.y + tilesize * tile.y;
    rect.right = mapmin.x + tilesize * (tile.x + 1);
    rect.top = mapmin.y + tilesize * (tile.y + 1);
    return rect;
}

parg_aar parg_aar_from_tilerange(parg_tilerange range, Vector2 mapsize)
{
    parg_aar a = parg_aar_from_tilename(range.mintile, mapsize);
    parg_aar b = parg_aar_from_tilename(range.maxtile, mapsize);
    return parg_aar_encompass(a, b);
}

parg_aar parg_aar_encompass(parg_aar a, parg_aar b)
{
    a.left = PARG_MIN(a.left, b.left);
    a.bottom = PARG_MIN(a.bottom, b.bottom);
    a.right = PARG_MAX(a.right, b.right);
    a.top = PARG_MAX(a.top, b.top);
    return a;
}

parg_aar parg_aar_stretch_to_square(parg_aar rect)
{
    float x = 0.5 * (rect.right + rect.left);
    float y = 0.5 * (rect.top + rect.bottom);
    float w = parg_aar_width(rect);
    float h = parg_aar_height(rect);
    if (w > h) {
        h = w / 2;
        rect.bottom = y - h;
        rect.top = y + h;
    } else {
        w = h / 2;
        rect.left = x - w;
        rect.right = x + w;
    }
    return rect;
}

float parg_aar_height(parg_aar rect) { return rect.top - rect.bottom; }

float parg_aar_width(parg_aar rect) { return rect.right - rect.left; }

float parg_aar_centerx(parg_aar rect) { return 0.5 * (rect.left + rect.right); }

float parg_aar_centery(parg_aar rect) { return 0.5 * (rect.top + rect.bottom); }
