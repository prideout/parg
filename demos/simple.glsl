
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform vec4 u_color;
varying vec4 v_color;

-- vertex

attribute vec4 a_position;

void main()
{
    gl_Position = u_mvp * a_position;
    v_color = u_color;
}

-- fragment

void main()
{
    gl_FragColor = v_color;
}
