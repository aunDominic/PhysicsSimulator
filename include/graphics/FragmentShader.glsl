#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec3 ViewPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 colorTop;
uniform vec3 colorBottom;
uniform float ambientStrength;

out vec4 FragColor;

void main()
{
    // Improved lighting calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Ambient light
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse light with softer falloff
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular highlights
    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Rim lighting for edge definition
    float rimAmount = 0.7;
    float rimThreshold = 0.1;
    float rimDot = 1.0 - dot(viewDir, norm);
    float rim = smoothstep(rimThreshold - 0.01, rimThreshold + 0.01, rimDot);
    vec3 rimLight = rim * rimAmount * lightColor;

    // Enhanced gradient
    float heightFactor = smoothstep(-1.0, 1.0, FragPos.y);
    vec3 gradientColor = mix(colorBottom, colorTop, heightFactor);

    // Combine all lighting components
    vec3 resultColor = (ambient + diffuse + specular + rimLight) * gradientColor;
    
    // Tone mapping for better contrast
    resultColor = resultColor / (resultColor + vec3(1.0));
    
    FragColor = vec4(resultColor, 1.0);
}