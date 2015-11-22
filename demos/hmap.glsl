
// @program p_color, vertex, color
// @program p_gray, vertex, gray
// @program p_black, vertex, black

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

-- color

uniform sampler2D img;

void main()
{
    vec4 texel = texture2D(img, v_texcoord);
    vec3 back = vec3(1);
    gl_FragColor.rgb = mix(back, texel.rgb, texel.a);
    gl_FragColor.a = 1.0;
}

-- gray

uniform sampler2D img;

void main()
{
    vec4 texel = texture2D(img, v_texcoord);
    #ifdef GRAY
    gl_FragColor.rgb = 0.5 * vec3(texel.a + 1.0);
    #else
    gl_FragColor.rgb = vec3(step(texel.a, 0.0));
    #endif
    gl_FragColor.a = 1.0;
}

-- black

void main()
{
    gl_FragColor = vec4(0, 0, 0, 1);
}
