#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform sampler2D diffuseTexture;
uniform bool useTexture;
uniform bool isSun;
uniform float emissionStrength;

void main() {
    if (isSun) {
        vec3 texColor = useTexture ? texture(diffuseTexture, TexCoords).rgb : objectColor;
        // ƒл€ —олнца €ркий эмиссионный цвет
        FragColor = vec4(texColor * emissionStrength, 1.0);
        return;
    }
    
    // освещение по ‘онгу дл€ остальных объектов
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * objectColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0);
    
    vec3 lighting = ambient + diffuse + specular;
    vec3 baseColor = useTexture ? texture(diffuseTexture, TexCoords).rgb : objectColor;
    FragColor = vec4(lighting * baseColor, 1.0);
}
