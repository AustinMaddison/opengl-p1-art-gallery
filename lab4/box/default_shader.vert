#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;
uniform float uTime;

mat3x3 rotate_mat_z(float t)
{
     return mat3x3(
        cos(t), -sin(t), 0.0f, 
        sin(t),  cos(t), 0.0f, 
        0.0f,    0.0f,   1.0f);
}

mat3x3 rotate_mat_x(float t)
{
     return mat3x3(
        1.f,    0.0f,       0.0f, 
        0.0f,   sin(t),  cos(t), 
        0.0f,   cos(t), -sin(t));
}


mat3x3 rotate_mat_y(float t)
{
     return mat3x3(
        cos(t), 0,  sin(t), 
        0,      1.0f,  0, 
        -sin(t),   0, cos(t));
}


void main()
{ 
    gl_Position = vec4(aPos * rotate_mat_z(uTime) * rotate_mat_x(uTime), 1.0);
    // gl_Position = vec4(aPos, 1);
    // gl_Position = vec4(aPos * rotate_mat_y(uTime), 1.0);
    // gl_Position = vec4(aPos * rotate_mat_x(uTime), 1.0);
    vColor = aPos.xyz;
}
