
// @program p_color, vertex, color
// @program p_gray, vertex, gray
// @program p_graymesh, vertex, mesh
// @program p_colormesh, vertexcolor, colormesh

uniform mat4 u_mvp;
uniform vec4 u_color;
varying vec2 v_texcoord;

-- vertex

attribute vec4 a_position;
varying float v_brightness;
attribute vec2 a_texcoord;

void main()
{
    vec4 p = a_position;
    gl_Position = u_mvp * p;
    v_brightness = a_position.z * 2.0;
    v_texcoord = a_texcoord;
}

-- vertexcolor

attribute vec4 a_position;
varying float v_brightness;

void main()
{
    vec4 p = a_position;
    p.z *= 0.25;
    gl_Position = u_mvp * p;
    v_brightness = a_position.z;
    v_texcoord = a_position.xy;
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
    float L = 0.75 * (texel.a + 1.0);
    gl_FragColor.rgb = vec3(L * L);
    gl_FragColor.a = 1.0;
}

-- mesh

varying float v_brightness;

void main()
{
    vec3 base = u_color.rgb;
    gl_FragColor.rgb = mix(base, vec3(1), v_brightness);
    gl_FragColor.a = 1.0;
}

-- colormesh

uniform sampler2D img;
varying float v_brightness;

void main()
{
    vec4 texel = texture2D(img, v_texcoord);
    gl_FragColor.rgb = texel.rgb * v_brightness; // mix(texel.rgb, vec3(1), v_brightness);
    gl_FragColor.a = 1.0;
}
