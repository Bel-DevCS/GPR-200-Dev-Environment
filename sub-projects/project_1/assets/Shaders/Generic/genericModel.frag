#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightDir;         // Direction of the light
uniform vec3 lightColor;       // Color of the light
uniform vec3 materialColor;    // Color from the material
uniform int useMaterialColor;  // Flag to use material color

void main()
{
    // Normalize vectors
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(lightDir);

    // Diffuse shading
    float diff = max(dot(norm, -lightDirNorm), 0.0);
    vec3 diffuse = lightColor * diff;

    // Calculate the final color
    vec3 finalColor = materialColor * diffuse;

    if (useMaterialColor == 1)
    {
        FragColor = vec4(finalColor, 1.0);
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Fallback, no texture support yet
    }
}
