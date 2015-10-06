
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;

-- vertex

attribute vec4 a_position;

void main()
{
    gl_Position = u_mvp * a_position;
}

-- fragment

void main()
{
    gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);
}
