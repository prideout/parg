
// @program p_background, vertex, fragment

varying vec2 v_texcoord;

-- vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = a_position;
    v_texcoord = a_texcoord;
}

-- fragment

uniform sampler2D img;

void main()
{
    gl_FragColor = texture2D(img, v_texcoord);
}
