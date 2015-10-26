
// @program p_simple, vertex, fragment
// @program p_textured, vtexture, ftexture

uniform mat4 u_mvp;
uniform vec3 u_eyepos;
uniform float u_pointsize;
uniform float u_magnification;
uniform float u_density;
varying float v_pointsize;
varying float v_alpha;
varying vec2 v_texcoord;

-- vertex

attribute vec3 a_position;

const vec4 OUTSIDE_FRUSTUM = vec4(2, 2, 2, 1);
const float FADE_DURATION = 0.25;

void main()
{
    vec4 p = vec4(a_position.xy, 0.0, 1.0);
    gl_Position = u_mvp * p;
    gl_PointSize = u_pointsize;
    float rank = a_position.z * u_density;
    float mag2 = u_magnification * u_magnification;
    float culled = rank - mag2;

    v_alpha = smoothstep(0.0, - (FADE_DURATION * mag2), culled);

    v_texcoord = a_position.xy;
    v_texcoord.y *= 2.0 ;
    v_texcoord += vec2(0.5, 0.5);

    gl_PointSize *= v_alpha;
    v_pointsize = gl_PointSize;

    if (v_alpha == 0.0) {
        gl_Position = OUTSIDE_FRUSTUM;
    }
}

-- fragment

uniform sampler2D img;

void main()
{
    vec2 pc = 2.0 * (gl_PointCoord - 0.5);
    float r = dot(pc, pc);
    gl_FragColor.rgb = 0.3 + texture2D(img, v_texcoord).rgb;
    gl_FragColor.rgb *= smoothstep(1.0, 0.0, r);
    gl_FragColor.a = smoothstep(1.0, 0.9, r);
}

-- vtexture

attribute vec4 a_position;
attribute vec2 a_texcoord;

void main()
{
    gl_Position = u_mvp * a_position;
    v_texcoord = a_texcoord;
}

-- ftexture

uniform sampler2D img;

float sample(vec2 uv)
{
    vec4 c = texture2D(img, uv);
    return c.r + c.g + c.b;
}

float computeSobelFilter(vec2 uv)
{
    float h = 0.5 / 2048.0;
    float t00 = sample(uv + h * vec2(-1, -1));
    float t10 = sample(uv + h * vec2( 0, -1));
    float t20 = sample(uv + h * vec2( 1, -1));
    float t01 = sample(uv + h * vec2(-1, 0));
    float t21 = sample(uv + h * vec2( 1, 0));
    float t02 = sample(uv + h * vec2(-1, 1));
    float t12 = sample(uv + h * vec2( 0, 1));
    float t22 = sample(uv + h * vec2( 1, 1));
    vec2 grad;
    grad.x = t00 + 2.0 * t01 + t02 - t20 - 2.0 * t21 - t22;
    grad.y = t00 + 2.0 * t10 + t20 - t02 - 2.0 * t12 - t22;
    return step(0.01, length(grad));
}

void main()
{
    gl_FragColor = texture2D(img, v_texcoord);
    gl_FragColor.rgb *= 1.0 -
        0.5 *
        smoothstep(12.0, 8.0, u_magnification) *
        computeSobelFilter(v_texcoord);
}
