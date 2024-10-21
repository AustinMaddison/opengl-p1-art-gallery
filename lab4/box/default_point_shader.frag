#version 330 core  
out vec4 FragColor;

uniform float uTime;

in vec2 vTexCoord;


void main()  
{  
    vec2 st = (gl_PointCoord.st - 0.5f) * 2.0f;
    float d = distance(vec2(0.0f) , st);

    d = step(0.9f, d);
    float alpha = 1.0f - smoothstep(0.99f, 1.01f, dot(d, d) * 8);

    FragColor = vec4(vec3(1.0f), alpha * 0.6); 
    // FragColor = vec4(vTexCoord, 0.0f, alpha);  
}