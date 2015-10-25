
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform vec3 u_eyepos;
uniform float u_magnification;
uniform float u_density;
varying float v_alpha;

-- vertex

attribute vec3 a_position;
attribute float a_vertexid;

const vec4 OUTSIDE_FRUSTUM = vec4(2, 2, 2, 1);
const float FADE_DURATION = 0.25;

void main()
{
    vec4 p = vec4(a_position.xy - 0.5, 0.0, 1.0);
    p.y *= -1.0;
    gl_Position = u_mvp * p;
    gl_PointSize = 10.0;
    float rank = a_vertexid * u_density;
    float mag2 = u_magnification * u_magnification;
    float culled = rank - mag2;
    v_alpha = smoothstep(0.0, - (FADE_DURATION * mag2), culled);
    gl_PointSize *= v_alpha;
    if (v_alpha == 0.0) {
        gl_Position = OUTSIDE_FRUSTUM;
    }
}

-- fragment

void main()
{
    float L = 0.8 - v_alpha * 0.6;
    gl_FragColor = vec4(L, L, L, 1.0);
}
