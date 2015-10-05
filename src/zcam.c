#include <par.h>

static Matrix4 _projmat;
static Point3 _camerapos;
static Vector3 _worldsize;
static float _maxcamz;
static float _mincamz;
static float _fovy;
static float _winaspect;
static float _zplanes[2];
static Point3 _grabpt;
static int _grabbing = 0;

#define MIN(a, b) (a > b ? b : a)
#define MAX(a, b) (a > b ? a : b)
#define CLAMP(v, lo, hi) MAX(lo, MIN(hi, v))

static Point3 window_to_world(float winx, float winy)
{
    Point3 worldspace;
    float vpheight = 2 * tan(_fovy / 2) * _camerapos.z;
    float vpwidth = vpheight * _winaspect;
    worldspace.y = _camerapos.y + vpheight * (winy - 0.5);
    worldspace.x = _camerapos.x + vpwidth * (winx - 0.5);
    worldspace.z = 0;
    return worldspace;
}

void par_zcam_init(float worldwidth, float worldheight, float fovy)
{
    _maxcamz = 0.5 * worldheight / tan(fovy * 0.5);
    _camerapos = (Point3){0, 0, _maxcamz};
    _mincamz = 0.01;
    _zplanes[0] = _mincamz * 0.9;
    _zplanes[1] = _maxcamz * 1.1;
    _fovy = fovy;
    _worldsize.x = worldwidth;
    _worldsize.y = worldheight;
}

void par_zcam_tick(float winaspect, float seconds)
{
    _winaspect = winaspect;
    _projmat = M4MakePerspective(_fovy, _winaspect, _zplanes[0], _zplanes[1]);
}

void par_zcam_grab_begin(float winx, float winy)
{
    _grabbing = 1;
    _grabpt = window_to_world(winx, winy);
}

void par_zcam_grab_update(float winx, float winy, float scrolldelta)
{
    if (_grabbing) {
        float vpheight = 2 * tan(_fovy / 2) * _camerapos.z;
        float vpwidth = vpheight * _winaspect;
        _camerapos.y = -vpheight * (winy - 0.5) + _grabpt.y;
        _camerapos.x = -vpwidth * (winx - 0.5) + _grabpt.x;
    } else if (scrolldelta) {
        Point3 focalpt = window_to_world(winx, winy);
        _camerapos.z -= scrolldelta * _camerapos.z * 0.01;
        _camerapos.z = CLAMP(_camerapos.z, _mincamz, _maxcamz);
        float vpheight = 2 * tan(_fovy / 2) * _camerapos.z;
        float vpwidth = vpheight * _winaspect;
        _camerapos.y = -vpheight * (winy - 0.5) + focalpt.y;
        _camerapos.x = -vpwidth * (winx - 0.5) + focalpt.x;
    }
}

void par_zcam_grab_release() { _grabbing = 0; }

void par_zcam_matrices(Matrix4* proj, Matrix4* view)
{
    *proj = _projmat;
    Point3 target = {_camerapos.x, _camerapos.y, 0};
    Vector3 up = {0, 1, 0};
    *view = M4MakeLookAt(_camerapos, target, up);
}
