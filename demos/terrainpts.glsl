
// @program p_simple, vertex, fragment
// @program p_textured, vertex, textured

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

void main()
{
    vec2 pc = 2.0 * (gl_PointCoord - 0.5);
    float r = dot(pc, pc);
    gl_FragColor = vec4(0.2, 0.2, 0.2, v_alpha);
    gl_FragColor.a *= smoothstep(1.0, 0.9, r);
}

-- textured

uniform sampler2D img;

void main()
{
    vec2 pc = 2.0 * (gl_PointCoord - 0.5);
    float r = dot(pc, pc);
    vec2 uv = v_texcoord;
    gl_FragColor = texture2D(img, uv);
    gl_FragColor.a = smoothstep(1.0, 0.9, r);
}
