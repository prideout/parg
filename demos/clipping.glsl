
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform mat3 u_imv;
uniform vec4 u_color;
varying vec3 v_normal;

-- vertex

attribute vec4 a_position;
attribute vec3 a_normal;

void main()
{
    gl_Position = u_mvp * a_position;
    v_normal = u_imv * a_normal;
}

-- fragment

void main()
{
    gl_FragColor = u_color * dot(v_normal, vec3(0, 0, 1));
}
