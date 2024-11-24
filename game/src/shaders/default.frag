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
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance = length(quantize(light.position, 16.0f) - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    

    float steps = 10.0f;
    float attenuation_c = 0.0f;
    for(float i = 1.0f; i <= steps; i+=1.0f)
    {
        float offset = 0.8f/steps *i;
        attenuation_c += smoothstep(offset - 0.01, offset, attenuation) * sqrt(i);
        attenuation_c += (smoothstep(offset - 0.01, offset, attenuation) - smoothstep(offset - 0.1, offset-0.02, attenuation))  * sqrt(i);
    }

    attenuation_c /= steps;
    ambient *= attenuation;  
    diffuse *= attenuation_c;  
    specular *= attenuation_c;  



    return ambient + diffuse + specular;
}


void main()
{
    // texture sample space
    vec3 uvw = vec3(1.0f) * material.scale + material.translate;
    if (material.sampleSpace == 0) uvw *= vec3(TexCoords, 0.0);
    if (material.sampleSpace == 1) uvw *= vec3(FragPos.xz, 0.0);
    if (material.sampleSpace == 2) uvw *= vec3(FragPos.xy, 0.0);
    if (material.sampleSpace == 3) uvw *= vec3(FragPos.zy, 0.0);
    vec2 uv = uvw.xy;

    vec3 color = vec3(0.0);
    vec3 fragPos = quantize(FragPos, 16.0f);
    
    for(int i = 0; i < MAX_LIGHTS; i ++)
    {
        color += calcLight(lights[i], Normal, fragPos, viewPos, uv);
    }

    // vec3 viewDir = normalize(viewPos - FragPos);
    // viewDir = quantize(viewDir, 3.0f);
    // vec3 reflectDir = reflect(-lightDir, Normal);  

    FragColor = vec4(color, 1.0);
    // FragColor = vec4(quantize(FragPos, 16.0f), 1.0);
} 