
// @program p_textured, vertex, fragment
// @program p_solid, vertex, solid

uniform mat4 u_mvp;
varying vec2 v_texcoord;

-- vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = u_mvp * a_position;
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
