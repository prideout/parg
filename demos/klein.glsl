
// @program p_texture, klein.vertex, texture
// @program p_cylinder, klein.vertex, cylinder.fragment
// @program p_podium, klein.vertex, podium.fragment
// @program p_logo, logo.vertex, texture

uniform mat4 u_mvp;
uniform sampler2D u_ao;
uniform sampler2D u_rust;
uniform sampler2D u_reflection;
uniform vec2 u_resolution;
varying vec2 v_texcoord;
varying vec3 v_position;

-- klein.vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texcoord = a_texcoord;
    v_position = a_position.xyz;
}

-- logo.vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = a_position;
    gl_Position.xy *= 0.4;
    gl_Position.xy += 0.7;
    v_texcoord = a_texcoord + 0.5;
}

-- texture

void main()
{
    gl_FragColor = texture2D(u_ao, v_texcoord);
}

-- cylinder.fragment

void main()
{
    gl_FragColor = vec4(0.56, 0.31, 0.17, 1.0);
}

-- podium.fragment

void main()
{
    gl_FragColor = texture2D(u_ao, v_texcoord);
    gl_FragColor *= texture2D(u_rust, v_position.xz);
    vec4 reflection = texture2D(u_reflection, gl_FragCoord.xy / u_resolution);
    reflection.a *= gl_FragCoord.z;
    gl_FragColor.rgb = mix(gl_FragColor.rgb, reflection.rgb, reflection.a);
}
