#version 330 core  
out vec4 FragColor;

in vec2 fragmentTexCoord;
uniform sampler2D material;

uniform vec2 texScale;
uniform vec2 texTransform;

void main()  
{  
    FragColor = vec4(texture(material, fragmentTexCoord * texScale + texTransform).rgb, 1.0); 
}