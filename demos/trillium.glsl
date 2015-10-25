
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform float u_magnification;
varying float v_alpha;

-- vertex

attribute vec3 a_position;

void main()
{
    vec4 p = vec4(a_position.xy, 0.0, 1.0);
    gl_Position = u_mvp * p;
    v_alpha = 1.0;
    gl_PointSize = 5.0 * v_alpha;
}

-- fragment

void main()
{
    float L = 0.8 - v_alpha * 0.6;
    gl_FragColor = vec4(L, L, L, 1.0);
}
