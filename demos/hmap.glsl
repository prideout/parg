
// @program p_simple, vertex, fragment

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
    // float luma = texture2D(img, v_texcoord).x + 0.5;
    // gl_FragColor = vec4(vec3(luma), 1);
}
