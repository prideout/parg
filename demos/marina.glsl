
// @program p_textured, vertex, fragment
// @program p_highp, vertex_highp, solid

uniform mat4 u_mvp;
uniform vec3 u_eyepos;
uniform vec3 u_eyepos_lowpart;
varying vec2 v_texcoord;

-- vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texcoord = a_texcoord;
}

-- vertex_highp

attribute vec3 a_position_lowpart;
attribute vec3 a_position;
attribute vec2 a_texcoord;

void main()
{
    vec3 t1 = a_position_lowpart - u_eyepos_lowpart;
    vec3 e = t1 - a_position_lowpart;
    vec3 t2 = ((-u_eyepos_lowpart - e) + (a_position_lowpart - (t1 - e))) + a_position - u_eyepos;
    vec3 high_delta = t1 + t2;
    vec3 low_delta = t2 - (high_delta - t1);
    vec3 p = high_delta + low_delta;
    gl_Position = u_mvp * vec4(p, 1.0);
    v_texcoord = a_texcoord;
}

-- fragment

uniform sampler2D img;

void main()
{
    gl_FragColor = texture2D(img, v_texcoord);
}

-- solid

void main()
{
    gl_FragColor = vec4(0.2, 0.7, 0.8, 1.0);
}
