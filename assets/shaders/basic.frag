#version 330 core
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 uColor;
uniform sampler2D uTexture;
uniform int uUseTexture;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

out vec4 FragColor;

void main()
{
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * uLightColor;
  	
    // Diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;  
        
    vec3 lighting = (ambient + diffuse + specular);
    
    vec3 baseColor;
    if (uUseTexture == 1)
        baseColor = texture(uTexture, vTexCoord).rgb;
    else
        baseColor = uColor;

    FragColor = vec4(lighting * baseColor, 1.0);
}
