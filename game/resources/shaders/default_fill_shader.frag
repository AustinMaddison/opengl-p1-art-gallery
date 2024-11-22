#version 330 core  
out vec4 FragColor;

in vec2 vTexCoord;
in vec3 vWorldCoord;
    
uniform float uTime;
uniform vec3 uScaleTexCoords;
uniform vec3 uTransformTexCoords;
uniform bool uIsWorldSpaceTexCoord;

uniform sampler2D texture0;

void main()  
{  
    vec2 uv = vTexCoord * uScaleTexCoords.xy + uTransformTexCoords.xy;
    vec3 uvw = vWorldCoord * uScaleTexCoords + uTransformTexCoords;

    vec3 color = vec3(0.0f);

    if(!uIsWorldSpaceTexCoord)
    {
        color += texture(texture0, uv).rgb
    }
    else
    {
        color += texture(texture0, uvw).rgb
    }
    
    FragColor = vec4(color, 1.0f);  
in vec2 fragmentTexCoord;
uniform sampler2D material;

uniform vec2 texScale;
uniform vec2 texTransform;

void main()  
{  
    FragColor = vec4(texture(material, fragmentTexCoord * texScale + texTransform).rgb, 1.0); 
}