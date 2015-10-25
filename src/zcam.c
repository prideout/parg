#include <par.h>

static DMatrix4 _projmat;
static DPoint3 _camerapos;
static DVector3 _worldsize;
static double _maxcamz;
static double _mincamz;
static double _fovy;
static double _winaspect = 0;
static double _zplanes[2];
static DPoint3 _grabpt;
static int _grabbing = 0;

#define MIN(a, b) (a > b ? b : a)
#define MAX(a, b) (a > b ? a : b)
#define CLAMP(v, lo, hi) MAX(lo, MIN(hi, v))

static DPoint3 window_to_world(float winx, float winy)
{
    DPoint3 worldspace;
    double vpheight = 2 * tan(_fovy / 2) * _camerapos.z;
    double vpwidth = vpheight * _winaspect;
    worldspace.y = _camerapos.y + vpheight * (winy - 0.5);
    worldspace.x = _camerapos.x + vpwidth * (winx - 0.5);
    worldspace.z = 0;
    return worldspace;
}

void par_zcam_init(float worldwidth, float worldheight, float fovy)
{
    _maxcamz = 0.5 * worldheight / tan(fovy * 0.5);
    _camerapos = (DPoint3){0, 0, _maxcamz};
    _mincamz = 0.0000001;
    _zplanes[0] = _mincamz * 0.9;
    _zplanes[1] = _maxcamz * 1.1;
    _fovy = fovy;
    _worldsize.x = worldwidth;
    _worldsize.y = worldheight;
}

void par_zcam_tick(float winaspect, float seconds)
{
    if (_winaspect != winaspect) {
        _winaspect = winaspect;
        double* z = _zplanes;
        _projmat = DM4MakePerspective(_fovy, _winaspect, z[0], z[1]);
    }
}

void par_zcam_grab_begin(float winx, float winy)
{
    _grabbing = 1;
    _grabpt = window_to_world(winx, winy);
}

void par_zcam_grab_update(float winx, float winy, float scrolldelta)
{
    if (_grabbing) {
        double vpheight = 2 * tan(_fovy / 2) * _camerapos.z;
        double vpwidth = vpheight * _winaspect;
        _camerapos.y = -vpheight * (winy - 0.5) + _grabpt.y;
        _camerapos.x = -vpwidth * (winx - 0.5) + _grabpt.x;
    } else if (scrolldelta) {
        DPoint3 focalpt = window_to_world(winx, winy);
        _camerapos.z -= scrolldelta * _camerapos.z * 0.01;
        _camerapos.z = CLAMP(_camerapos.z, _mincamz, _maxcamz);
        double vpheight = 2 * tan(_fovy / 2) * _camerapos.z;
        double vpwidth = vpheight * _winaspect;
        _camerapos.y = -vpheight * (winy - 0.5) + focalpt.y;
        _camerapos.x = -vpwidth * (winx - 0.5) + focalpt.x;
    }
}

void par_zcam_grab_end() { _grabbing = 0; }

DPoint3 par_zcam_dmatrices(DMatrix4* proj, DMatrix4* view)
{
    *proj = _projmat;
    DPoint3 target = {_camerapos.x, _camerapos.y, 0};
    DVector3 up = {0, 1, 0};
    *view = DM4MakeLookAt(_camerapos, target, up);
    return _camerapos;
}

Point3 par_zcam_matrices(Matrix4* proj, Matrix4* view)
{
    *proj = M4MakeFromDM4(_projmat);
    DPoint3 target = {_camerapos.x, _camerapos.y, 0};
    DVector3 up = {0, 1, 0};
    *view = M4MakeFromDM4(DM4MakeLookAt(_camerapos, target, up));
    return (Point3){_camerapos.x, _camerapos.y, _camerapos.z};
}

void par_zcam_highprec(Matrix4* vp, Point3* eyepos_lo, Point3* eyepos_hi)
{
    DPoint3 origin = {0, 0, 0};
    DPoint3 target = {0, 0, -1};
    DVector3 up = {0, 1, 0};
    DMatrix4 view = DM4MakeLookAt(origin, target, up);
    if (vp) {
        *vp = M4MakeFromDM4(DM4Mul(_projmat, view));
    }
    Point3 eyepos = P3MakeFromDP3(_camerapos);
    DPoint3 deyepos = DP3MakeFromP3(eyepos);
    DVector3 difference = DP3Sub(_camerapos, deyepos);
    if (eyepos_lo) {
        *eyepos_lo = P3MakeFromV3(V3MakeFromDV3(difference));
    }
    *eyepos_hi = eyepos;
}
