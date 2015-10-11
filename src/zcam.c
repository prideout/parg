#include <par.h>

static DMatrix4 _projmat;
static DPoint3 _camerapos;
static DVector3 _worldsize;
static double _maxcamz;
static double _mincamz;
static double _fovy;
static double _winaspect;
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
    _winaspect = winaspect;
    _projmat = DM4MakePerspective(_fovy, _winaspect, _zplanes[0], _zplanes[1]);
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

void par_zcam_grab_release() { _grabbing = 0; }

void par_zcam_dmatrices(DMatrix4* proj, DMatrix4* view)
{
    *proj = _projmat;
    DPoint3 target = {_camerapos.x, _camerapos.y, 0};
    DVector3 up = {0, 1, 0};
    *view = DM4MakeLookAt(_camerapos, target, up);
}

static Matrix4 double_to_float(DMatrix4 dmat)
{
    Matrix4 m;
    m.col0.x = dmat.col0.x;
    m.col0.y = dmat.col0.y;
    m.col0.z = dmat.col0.z;
    m.col0.w = dmat.col0.w;
    m.col1.x = dmat.col1.x;
    m.col1.y = dmat.col1.y;
    m.col1.z = dmat.col1.z;
    m.col1.w = dmat.col1.w;
    m.col2.x = dmat.col2.x;
    m.col2.y = dmat.col2.y;
    m.col2.z = dmat.col2.z;
    m.col2.w = dmat.col2.w;
    m.col3.x = dmat.col3.x;
    m.col3.y = dmat.col3.y;
    m.col3.z = dmat.col3.z;
    m.col3.w = dmat.col3.w;
    return m;
}

void par_zcam_matrices(Matrix4* proj, Matrix4* view)
{
    *proj = double_to_float(_projmat);
    DPoint3 target = {_camerapos.x, _camerapos.y, 0};
    DVector3 up = {0, 1, 0};
    *view = double_to_float(DM4MakeLookAt(_camerapos, target, up));
}
