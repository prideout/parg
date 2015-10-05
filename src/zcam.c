#include <par.h>

static Matrix4 _projmat;
static Point3 _camerapos;
static Vector3 _worldsize;
static float _fovy;
static float _winaspect;
static float _zplanes[2];

void par_zcam_init(float worldwidth, float worldheight, float fovy)
{
    float cameraheight = 0.5 * worldheight / tan(fovy * 0.5);
    _camerapos = (Point3){0, 0, cameraheight};
    _zplanes[0] = 1.0;
    _zplanes[1] = cameraheight * 1.1;
    _fovy = fovy;
    _worldsize.x = worldwidth;
    _worldsize.y = worldheight;
}

void par_zcam_tick(float winaspect, float seconds)
{
    _winaspect = winaspect;
    _projmat = M4MakePerspective(_fovy, _winaspect, _zplanes[0], _zplanes[1]);
}

void par_zcam_grab_begin(float winx, float winy) {}

void par_zcam_grab_update(float winx, float winy, float scrolldelta) {}

void par_zcam_grab_release() {}

void par_zcam_matrices(Matrix4* proj, Matrix4* view)
{
    *proj = _projmat;
    Point3 target = {_camerapos.x, _camerapos.y, 0};
    Vector3 up = {0, 1, 0};
    *view = M4MakeLookAt(_camerapos, target, up);
}
