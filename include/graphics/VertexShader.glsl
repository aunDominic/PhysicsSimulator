#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 ViewPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Better normal transformation using transpose inverse
    Normal = transpose(inverse(mat3(model))) * aNormal;
    
    // Pass view position for specular calculation
    ViewPos = vec3(view[3]);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}