
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;

-- vertex

attribute vec3 a_position;

void main()
{
    vec4 p = vec4(a_position.xy - 0.5, 0.0, 1.0);
    p.y *= -1.0;
    gl_Position = u_mvp * p;
    gl_PointSize = 2.0;
}

-- fragment

void main()
{
    gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);
}
