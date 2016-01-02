
// @program p_simple, vertex, fragment
// @program p_texture, vertex, texture

uniform mat4 u_mvp;
uniform mat3 u_imv;
uniform sampler2D u_img;
varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_position;

-- vertex

attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;

void main()
{
    gl_Position = u_mvp * a_position;
    v_normal = u_imv * a_normal;
    v_texcoord = a_texcoord;
}

-- fragment

#define GAMMA vec3(2.2)

vec3 LightPosition = vec3(0.5, 0.25, 1.0);
vec3 AmbientMaterial = pow(vec3(0.04, 0.04, 0.04), GAMMA);
vec3 SpecularMaterial = pow(vec3(0.5, 0.5, 0.5), GAMMA);
vec3 FrontMaterial = pow(vec3(0.75, 0.5, 0.25), GAMMA);
float Shininess = 50.0;

void main()
{
    vec3 N = normalize(v_normal);
    vec3 L = normalize(LightPosition);
    vec3 Eye = vec3(0, 0, 1);
    vec3 H = normalize(L + Eye);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);
    vec3 lighting = AmbientMaterial + df * FrontMaterial;
    lighting += sf * SpecularMaterial;
    lighting = pow(lighting, 1.0 / GAMMA);
    gl_FragColor = vec4(lighting, 1.0);
}

-- texture

void main()
{
    gl_FragColor = texture2D(u_img, v_texcoord);
}
