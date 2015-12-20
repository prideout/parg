
// @program p_landmass, vertex, landmass
// @program p_ocean, vertex, ocean
// @program p_solid, vertex, solid

uniform mat4 u_mvp;
uniform float u_magnification;
uniform sampler2D u_texture;
varying vec2 v_texcoord;

-- vertex

attribute vec4 a_position;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texcoord = a_position.xy;
}

-- landmass

vec4 sample(vec2 uv)
{
    return texture2D(u_texture, uv);
}

void main()
{
    const vec3 COLOR = vec3(0.55, 0.73, 0.61);
    float mag = u_magnification;
    float steps = 1.0;
    float freq0 = floor(mag * steps);
    float freq1 = ceil(mag * steps);
    float t = fract(mag * steps);
    vec4 c0 = sample(2.0 * v_texcoord * freq0 / steps);
    vec4 c1 = sample(2.0 * v_texcoord * freq1 / steps);
    gl_FragColor = mix(c0, c1, t) * vec4(COLOR, 1.0);
}

-- ocean

void main()
{
    gl_FragColor = texture2D(u_texture, 16.0 * v_texcoord);
}

-- solid

void main()
{
    gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0);
}
