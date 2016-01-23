
// @program p_simple, vertex, fragment

#extension GL_OES_standard_derivatives : enable

uniform mat4 u_mvp;
uniform float u_sel;
varying float v_rim;
varying vec3 v_fill;
varying float v_alpha;
const float STROKEW = 0.98;
const vec3 FILLC = vec3(1);
const vec3 STROKEC = vec3(0);
const vec3 SELC = vec3(1, 1, 0);

-- vertex

attribute vec4 a_position;

void main()
{
    vec4 p = a_position;
    v_rim = 1.0 - 2.0 * fract(a_position.z);
    bool selected = floor(a_position.z) == u_sel;
    v_fill = selected ? SELC : FILLC;
    v_alpha = selected ? 0.4 : 0.2;
    p.z = 0.0;
    gl_Position = u_mvp * p;
}

-- fragment

void main()
{
    float fw = fwidth(v_rim);
    float e = smoothstep(STROKEW - fw, STROKEW + fw, v_rim);
    vec3 v = mix(v_fill, STROKEC, e);
    float a = mix(v_alpha, 1.0, e);
    gl_FragColor = vec4(v, a);
}
