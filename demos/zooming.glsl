
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

const float brightness = 0.9;

void main()
{
    float luma = texture2D(img, v_texcoord).x;
    float fwid = fwidth(luma);
    luma = brightness * smoothstep(0.5 - fwid, 0.5 + fwid, luma);
    gl_FragColor = vec4(vec3(luma), 1);
}
