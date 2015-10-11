
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

attribute vec3 a_position;
attribute vec2 a_texcoord;

void main()
{
    vec3 p = a_position - u_eyepos;
    p -= u_eyepos_lowpart;
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
