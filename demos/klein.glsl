
// @program p_texture, klein.vertex, klein.fragment
// @program p_cylinder, klein.vertex, cylinder.fragment
// @program p_podium, klein.vertex, podium.fragment

uniform mat4 u_mvp;
uniform sampler2D u_ao;
uniform sampler2D u_rust;
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

-- klein.fragment

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
}
