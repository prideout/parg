
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform vec3 u_eyepos;

-- vertex

attribute vec3 a_position;
attribute float a_vertexid;

const vec4 OUTSIDE_FRUSTUM = vec4(2, 2, 2, 1);

void main()
{
    float rank = a_position.z;
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
