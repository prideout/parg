
// @program p_background, background.vs, background.fs
// @program p_asteroids, asteroids.vs, asteroids.fs
// @program p_particles, particles.vs, particles.fs

varying vec2 v_texcoord;
varying float v_index;

uniform float u_time;
uniform float u_npoints;
uniform sampler2D u_image;

const vec2 ATLAS_SIZE = vec2(160, 128);
const float SPRITE_SIZE = 32.0;
const float NSPRITES = 19.0;
const float NCOLS = 5.0;
const float ROTSPEED = 40.0;
const float BRIGHTEN = 1.75;
const float DARKEN = 1.0;
const float BUFSIZE = 256.0;

// http://www.iquilezles.org/www/articles/palettes/palettes.htm
vec3 select_color(float t)
{
    t /= u_npoints;
    const vec3 a = vec3(0.5, 0.5, 0.5);
    const vec3 b = vec3(0.5, 0.5, 0.5);
    const vec3 c = vec3(2.0, 1.0, 0.0);
    const vec3 d = vec3(0.50, 0.20, 0.25);
    return a + b * cos(6.28 * (c * t + d));
}

-- background.vs

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = a_position;
    v_texcoord = a_texcoord;
}

-- background.fs

void main()
{
    gl_FragColor = texture2D(u_image, v_texcoord);
    gl_FragColor.rgb *= DARKEN;
}

-- asteroids.vs

attribute vec4 a_position;

void main()
{
    gl_Position = a_position;
    v_index = a_position.z;
    gl_Position.z = 0.0;
    gl_PointSize = 50.0;
}

-- asteroids.fs

void main()
{
    float time = v_index * 2.0 + u_time * ROTSPEED;
    float sprite_index = floor(mod(time, NSPRITES));
    vec2 sprite = vec2(mod(sprite_index, NCOLS), floor(sprite_index / NCOLS));
    vec2 uv = sprite * SPRITE_SIZE + gl_PointCoord * SPRITE_SIZE;
    uv.y = ATLAS_SIZE.y - 1 - uv.y;
    gl_FragColor = texture2D(u_image, uv / ATLAS_SIZE);
    gl_FragColor.rgb *= select_color(v_index) * BRIGHTEN;
}

-- particles.vs

attribute float a_position;

void main()
{
    float u = mod(a_position, BUFSIZE);
    float v = floor(a_position / BUFSIZE);
    vec4 texel = texture2D(u_image, vec2(u, v) / BUFSIZE);
    gl_Position = vec4(texel.xy, 0, 1);
    gl_PointSize = 2.0;
}

-- particles.fs

void main()
{
    gl_FragColor = vec4(1);
}
