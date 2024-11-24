#version 330 core

precision mediump float;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    vec3 scale;
    vec3 translate;
    int sampleSpace;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 5

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
// uniform float time;
uniform Material material;
uniform Light lights[MAX_LIGHTS];

vec3 quantize(vec3 v, float factor)
{
    v = floor(v * factor + 0.5) / factor;
    return v;
}

//	Classic Perlin 3D Noise
//	by Stefan Gustavson (https://github.com/stegu/webgl-noise)
vec4 permute(vec4 x) {
    return mod(((x * 34.0) + 1.0) * x, 289.0);
}
vec4 taylorInvSqrt(vec4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}
vec3 fade(vec3 t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float cnoise(vec3 P) {
    vec3 Pi0 = floor(P); // Integer part for indexing
    vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
    Pi0 = mod(Pi0, 289.0);
    Pi1 = mod(Pi1, 289.0);
    vec3 Pf0 = fract(P); // Fractional part for interpolation
    vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
    vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    vec4 iy = vec4(Pi0.yy, Pi1.yy);
    vec4 iz0 = Pi0.zzzz;
    vec4 iz1 = Pi1.zzzz;

    vec4 ixy = permute(permute(ix) + iy);
    vec4 ixy0 = permute(ixy + iz0);
    vec4 ixy1 = permute(ixy + iz1);

    vec4 gx0 = ixy0 / 7.0;
    vec4 gy0 = fract(floor(gx0) / 7.0) - 0.5;
    gx0 = fract(gx0);
    vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
    vec4 sz0 = step(gz0, vec4(0.0));
    gx0 -= sz0 * (step(0.0, gx0) - 0.5);
    gy0 -= sz0 * (step(0.0, gy0) - 0.5);

    vec4 gx1 = ixy1 / 7.0;
    vec4 gy1 = fract(floor(gx1) / 7.0) - 0.5;
    gx1 = fract(gx1);
    vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
    vec4 sz1 = step(gz1, vec4(0.0));
    gx1 -= sz1 * (step(0.0, gx1) - 0.5);
    gy1 -= sz1 * (step(0.0, gy1) - 0.5);

    vec3 g000 = vec3(gx0.x, gy0.x, gz0.x);
    vec3 g100 = vec3(gx0.y, gy0.y, gz0.y);
    vec3 g010 = vec3(gx0.z, gy0.z, gz0.z);
    vec3 g110 = vec3(gx0.w, gy0.w, gz0.w);
    vec3 g001 = vec3(gx1.x, gy1.x, gz1.x);
    vec3 g101 = vec3(gx1.y, gy1.y, gz1.y);
    vec3 g011 = vec3(gx1.z, gy1.z, gz1.z);
    vec3 g111 = vec3(gx1.w, gy1.w, gz1.w);

    vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;

    float n000 = dot(g000, Pf0);
    float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
    float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
    float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
    float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
    float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
    float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
    float n111 = dot(g111, Pf1);

    vec3 fade_xyz = fade(Pf0);
    vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
    vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
    float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
    return 2.2 * n_xyz;
}

vec3 calcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewPos, vec2 uv)
{

    // fragPos = quantize(fragPos, 1.0f);

    normal = normalize(normal);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, uv).rgb;

    // diffuse
    vec3 lightDir = normalize(quantize(light.position, 16.0f) - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, uv).rgb;

    // specular
    vec3 viewDir = normalize(quantize(viewPos, 16.0f) - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, uv).rgb;

    // spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(quantize(light.position, 16.0f) - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float steps = 10.0f;
    float attenuation_c = 0.0f;
    for (float i = 1.0f; i <= steps; i += 1.0f)
    {
        float offset = 0.8f / steps * i;
        attenuation_c += smoothstep(offset - 0.01, offset, attenuation) * sqrt(i);
        attenuation_c += (smoothstep(offset - 0.01, offset, attenuation) - smoothstep(offset - 0.1, offset - 0.02, attenuation)) * sqrt(i);
    }

    attenuation_c /= steps;
    ambient *= attenuation;
    diffuse *= attenuation_c;
    specular *= attenuation_c;

    ambient = mix(ambient, vec3(0.0), cnoise(fragPos/2.0f)* 0.7 * cnoise(fragPos) + cnoise(fragPos*5.f)*0.3 );

    return ambient + diffuse + specular;
}



vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 colorGrade(vec3 color, float contrast, float highlights, float shadows) {
    // Apply contrast
    color = ((color - 0.5) * contrast + 0.5);

    // Apply highlights
    color = mix(color, vec3(1.0), highlights * smoothstep(0.5, 1.0, color));

    // Apply shadows
    color = mix(color, vec3(0.0), shadows * smoothstep(0.0, 0.5, color));

    return color;
}

void main()
{
    vec3 uvw = vec3(1.0f) * material.scale + material.translate;
    if (material.sampleSpace == 0) uvw *= vec3(TexCoords, 0.0);
    if (material.sampleSpace == 1) uvw *= vec3(FragPos.xz, 0.0);
    if (material.sampleSpace == 2) uvw *= vec3(FragPos.xy, 0.0);
    if (material.sampleSpace == 3) uvw *= vec3(FragPos.zy, 0.0);
    vec2 uv = uvw.xy;

    vec3 color = vec3(0.0);
    vec3 fragPos = quantize(FragPos, 16.0f);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        color += calcLight(lights[i], Normal, fragPos, viewPos, uv);
    }

    color = ACESFilm(color);
    color = colorGrade(color, 1.00, 0.001, 0.0);


    FragColor = vec4(color, 1.0);
    // FragColor = vec4(vec3(1.0)*d, 1.0);
}
