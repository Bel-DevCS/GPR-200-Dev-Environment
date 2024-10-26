#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 lightPos;
uniform float lightIntensity;
uniform int lightingModel; // 0 = Phong, 1 = Blinn-Phong

uniform vec3 materialColor;
uniform int useMaterialColor;

// New adjustable parameters
uniform float AmbientK;
uniform float DiffuseK;
uniform float SpecularK;
uniform float Shininess;

void main()
{
    // Ambient color
    vec3 ambient = AmbientK * lightColor;

    // Diffuse color
    vec3 norm = normalize(Normal);
    vec3 lightDirNormalized = normalize(lightDir);
    float diffStrength = max(dot(norm, lightDirNormalized), 0.0);
    vec3 diffuse = DiffuseK * diffStrength * lightColor;

    // Specular color
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirNormalized, norm);

    float spec = 0.0;
    if (lightingModel == 0) // Phong
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    }
    else if (lightingModel == 1) // Blinn-Phong
    {
        vec3 halfwayDir = normalize(lightDirNormalized + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), Shininess);
    }
    vec3 specular = SpecularK * spec * lightColor;

    // Calculate final color
    vec3 resultColor = ambient + diffuse + specular;
    resultColor *= (useMaterialColor == 1) ? materialColor : vec3(1.0);

    FragColor = vec4(resultColor, 1.0);
}
