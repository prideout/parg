
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;

-- vertex

attribute vec4 a_position;
attribute float a_vertexid;

void main()
{
    vec4 p = a_position;
    p.xy -= 0.5;
    p.y *= -1.0;
    gl_Position = u_mvp * p;
    gl_PointSize = 2.0;
}

-- fragment

void main()
{
    gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);
}
