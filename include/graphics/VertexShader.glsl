#version 120

attribute vec3 aPos;    // Vertex position
attribute vec3 aNormal; // Vertex normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec3 FragPos;  // Fragment position in world space
varying vec3 Normal;   // Normal vector in world space

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Simplified normal transformation
    Normal = mat3(model) * aNormal;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}