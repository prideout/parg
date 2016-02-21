
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform float u_sel;
uniform vec3 u_eyepos;
uniform vec3 u_eyepos_lowpart;
varying float v_rim;
varying vec3 v_fill;
varying float v_alpha;
const float STROKEW = 0.98;
const vec3 FILLC = vec3(1);
const vec3 STROKEC = vec3(0);
const vec3 SELC = vec3(1, 1, 0);

-- vertex

attribute vec3 a_position;
attribute vec4 a_center;

void main()
{
    vec3 cen = a_center.xyz;
    vec3 pos = vec3(a_position.xy * cen.z + cen.xy, 0.0);
    bool selected = a_center.w == u_sel;
    v_fill = selected ? SELC : FILLC;
    v_alpha = selected ? 0.4 : 0.2;
    v_rim = a_position.z;

    #ifdef SINGLE_PRECISION
        pos -= u_eyepos;
    #else
        vec3 poslow = vec3(0);
        vec3 t1 = poslow - u_eyepos_lowpart;
        vec3 e = t1 - poslow;
        vec3 t2 = ((-u_eyepos_lowpart - e) + (poslow - (t1 - e))) + pos - u_eyepos;
        vec3 high_delta = t1 + t2;
        vec3 low_delta = t2 - (high_delta - t1);
        pos = high_delta + low_delta;
    #endif

    gl_Position = u_mvp * vec4(pos, 1.0);
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
