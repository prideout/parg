
// @program p_texture, klein.vertex, klein.fragment

uniform mat4 u_mvp;
varying vec2 v_texcoord;

-- klein.vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texcoord = a_texcoord;
}

-- klein.fragment

uniform sampler2D ao;

void main()
{
    gl_FragColor = texture2D(ao, v_texcoord);
}
