#version 410 core

in vec3 FragPos;   // Fragment position in world space
in vec3 Normal;    // Normal vector in world space

uniform vec3 lightPos;      // Position of the light source
uniform vec3 lightColor;    // Color of the light
uniform vec3 colorTop;      // Top color of the gradient
uniform vec3 colorBottom;   // Bottom color of the gradient
uniform float ambientStrength; // Ambient light strength

out vec4 FragColor;

void main()
{
    // Normalize the normal and calculate the light direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // Ambient light
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse light (Lambertian reflection)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Calculate gradient color based on the Y position of the fragment
    float mixValue = (FragPos.y + 1.0) / 2.0; // Normalize Y position to [0, 1]
    vec3 gradientColor = mix(colorBottom, colorTop, mixValue);

    // Combine the ambient, diffuse, and gradient color
    vec3 resultColor = (ambient + diffuse) * gradientColor;

    FragColor = vec4(resultColor, 1.0);
}
