
// @program p_simple, vertex, fragment

uniform mat4 u_mvp;
uniform mat3 u_imv;
uniform float u_clipz;
varying vec3 v_normal;

-- vertex

attribute vec4 a_position;
attribute vec3 a_normal;

void main()
{
    gl_Position = u_mvp * a_position;
    v_normal = u_imv * a_normal;
}

-- fragment

vec3 LightPosition = vec3(0.25, 0.25, 1.0);
vec3 AmbientMaterial = vec3(0.04, 0.04, 0.04);
vec3 SpecularMaterial = vec3(0.5, 0.5, 0.5);
vec3 FrontMaterial = vec3(0.75, 0.75, 0.5);
vec3 BackMaterial = vec3(0.5, 0.5, 0.75);
float Shininess = 50.0;

void main()
{
    vec3 N = normalize(v_normal);

    if (gl_FrontFacing) {
        N = -N;
    }

    vec3 L = normalize(LightPosition);
    vec3 Eye = vec3(0, 0, 1);
    vec3 H = normalize(L + Eye);

    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    vec3 color = gl_FrontFacing ? FrontMaterial : BackMaterial;
    vec3 lighting = AmbientMaterial + df * color;
    if (gl_FrontFacing) {
        lighting += sf * SpecularMaterial;
    }

    if (gl_FragCoord.z < u_clipz) {
        discard;
    }

    gl_FragColor = vec4(lighting, 1.0);
}
