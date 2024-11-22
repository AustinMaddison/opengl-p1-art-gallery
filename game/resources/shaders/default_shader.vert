#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;
out vec3 vWorldCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;


void main()
{ 
    gl_Position = vec4(aPos * rotate_mat_z(uTime) * rotate_mat_x(uTime), 1.0);
    vTexCoord = aTexCoord;
    vWorldCoord = aPos;

out vec2 fragmentTexCoord;

void main()
{ 
    gl_Position = vec4(aPos, 1.0);
    fragmentTexCoord = aPos.xy;
}
