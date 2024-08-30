#version 410 core

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;  // Fragment position in world space
out vec3 Normal;   // Normal vector in world space

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Simplified normal transformation
    Normal = mat3(model) * aNormal;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
