
// @program p_landmass, vertex, landmass
// @program p_landmass_fragcoord, vertex, landmass_fragcoord
// @program p_ocean, vertex, ocean
// @program p_solid, vertex, solid

uniform mat4 u_mvp;
uniform vec4 u_color;
uniform vec4 u_slippybox;
uniform float u_slippyfract;
uniform bool u_showgrid;
uniform sampler2D u_texture;
varying vec2 v_texcoord;

const float LANDMASS_TEXTURE_FREQUENCY = 4.0;
const float OCEAN_TEXTURE_FREQUENCY = 16.0;
const vec4 LANDMASS_COLOR = vec4(0.5, 0.7, 0.6, 1.0);

vec4 sample(vec2 uv)
{
    vec4 texel = texture2D(u_texture, uv);
    if (u_showgrid) {
        uv = mod(uv, vec2(1));
        vec2 del = abs(vec2(0.5) - uv);
        vec2 m = 0.5 * smoothstep(0.5, 0.45, del);
        texel *= m.x * m.y + 0.5;
    }
    return texel;
}

-- vertex

attribute vec4 a_position;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texcoord = a_position.xy;
}

-- ocean

void main()
{
    vec2 tex_offset = v_texcoord - u_slippybox.xy;
    vec2 uv = tex_offset * u_slippybox.zw;
    gl_FragColor = sample(uv * OCEAN_TEXTURE_FREQUENCY);
}

-- solid

void main()
{
    gl_FragColor = u_color;
}

-- landmass

void main()
{
    vec2 tex_offset = v_texcoord - u_slippybox.xy;
    vec2 uv = tex_offset * u_slippybox.zw;
    vec4 texel0 = sample(uv * LANDMASS_TEXTURE_FREQUENCY);
    vec4 texel1 = sample(uv * LANDMASS_TEXTURE_FREQUENCY * 2.0);
    vec4 mixed = mix(texel0, texel1, u_slippyfract);
    gl_FragColor = LANDMASS_COLOR * mixed;
}

-- landmass_fragcoord

void main()
{
    vec2 tex_offset = gl_FragCoord.xy - u_slippybox.xy;
    vec2 uv = tex_offset * u_slippybox.zw;
    vec4 texel0 = sample(uv * LANDMASS_TEXTURE_FREQUENCY);
    vec4 texel1 = sample(uv * LANDMASS_TEXTURE_FREQUENCY * 2.0);
    vec4 mixed = mix(texel0, texel1, u_slippyfract);
    gl_FragColor = LANDMASS_COLOR * mixed;
}
