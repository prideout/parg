
// @program p_simple, vertex, fragment
// @program p_points, vpoints, fpoints

#extension GL_OES_standard_derivatives : enable

uniform mat4 u_mvp;
uniform float u_pointsize;
uniform vec3 u_color;
uniform float u_alpha;
varying float v_rim;
varying vec3 v_fill;
const float STROKEW = 0.99;
const vec3 STROKEC = vec3(0);

-- vertex

attribute vec4 a_position;

void main()
{
    vec4 p = a_position;
    v_rim = 1.0 - 2.0 * fract(a_position.z);
    v_fill = u_color;
    p.z = 0.0;
    gl_Position = u_mvp * p;
}

-- fragment

void main()
{
    float fw = fwidth(v_rim);
    float e = smoothstep(STROKEW - fw, STROKEW + fw, v_rim);
    vec3 v = mix(v_fill, STROKEC, e);
    float a = mix(u_alpha, 1.0, e);
    gl_FragColor = vec4(v, a);
}

-- vpoints

attribute vec4 a_position;

void main()
{
    vec4 p = a_position;
    p.z = 0.0;
    gl_Position = u_mvp * p;
    gl_PointSize = u_pointsize;
}

-- fpoints

void main()
{
    gl_FragColor = vec4(0, 0, 0, 1);
}
