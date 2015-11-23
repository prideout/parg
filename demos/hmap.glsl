
// @program p_color, vertex, color
// @program p_gray, vertex, gray
// @program p_graymesh, vertex, mesh
// @program p_colormesh, vertexcolor, mesh

uniform mat4 u_mvp;
varying vec2 v_texcoord;

-- vertex

attribute vec4 a_position;
varying float brightness;
attribute vec2 a_texcoord;

void main()
{
    vec4 p = a_position;
    gl_Position = u_mvp * p;
    brightness = a_position.z * 2.0;
    v_texcoord = a_texcoord;
}

-- vertexcolor

attribute vec4 a_position;
varying float brightness;
attribute vec2 a_texcoord;

void main()
{
    vec4 p = a_position;
    p.z *= 0.25;
    gl_Position = u_mvp * p;
    brightness = a_position.z;
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

-- mesh

varying float brightness;

void main()
{
    vec3 base = vec3(0.0, 0.3, 0.2);
    gl_FragColor.rgb = mix(base, vec3(1), brightness);
    gl_FragColor.a = 1.0;
}
