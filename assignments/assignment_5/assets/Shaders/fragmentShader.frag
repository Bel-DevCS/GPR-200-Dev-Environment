//Roughly cloned from GenericModelShader (Located in Project_1)
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 Colour;
in vec2 TexCoord;

// Light and view properties
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 lightPos;
uniform float lightIntensity;
uniform float cutoff;        // Spotlight cutoff angle (in radians)
uniform int lightType;       // 0 = Directional, 1 = Point, 2 = Spotlight
uniform int lightingModel;   // 0 = Phong, 1 = Blinn-Phong

// Adjustable parameters
uniform float AmbientK;
uniform float DiffuseK;
uniform float SpecularK;
uniform float Shininess;

// Animation and color controls
uniform float uTime;
uniform sampler2D bgTex;
uniform vec4 cubeColour;
uniform float uOscillationOffset;

void main()
{
    // Normalize normal vector
    vec3 norm = normalize(Normal);

    // Compute ambient color
    vec3 ambient = AmbientK * lightColor * lightIntensity;

    // Initialize variables for diffuse and specular components
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Calculate lighting based on light type
    if (lightType == 0) // Directional light
    {
        vec3 lightDirNormalized = normalize(lightDir);
        float diffStrength = max(dot(norm, lightDirNormalized), 0.0);
        diffuse = DiffuseK * diffStrength * lightColor * lightIntensity;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDirNormalized, norm);
        float spec = 0.0;
        if (lightingModel == 0)
        spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
        else
        spec = pow(max(dot(norm, normalize(lightDirNormalized + viewDir)), 0.0), Shininess);

        specular = SpecularK * spec * lightColor * lightIntensity;
    }
    else if (lightType == 1) // Point light
    {
        vec3 lightDirToLight = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

        float diffStrength = max(dot(norm, lightDirToLight), 0.0);
        diffuse = DiffuseK * diffStrength * lightColor * attenuation * lightIntensity;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDirToLight, norm);
        float spec = 0.0;
        if (lightingModel == 0)
        spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
        else
        spec = pow(max(dot(norm, normalize(lightDirToLight + viewDir)), 0.0), Shininess);

        specular = SpecularK * spec * lightColor * attenuation * lightIntensity;
    }
    else if (lightType == 2) // Spotlight
    {
        vec3 lightDirToLight = normalize(lightPos - FragPos);
        float theta = dot(lightDirToLight, normalize(-lightDir));

        if (theta > cutoff)
        {
            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

            float diffStrength = max(dot(norm, lightDirToLight), 0.0);
            diffuse = DiffuseK * diffStrength * lightColor * attenuation * lightIntensity;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDirToLight, norm);
            float spec = 0.0;
            if (lightingModel == 0)
            spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
            else
            spec = pow(max(dot(norm, normalize(lightDirToLight + viewDir)), 0.0), Shininess);

            specular = SpecularK * spec * lightColor * attenuation * lightIntensity;
        }
    }

    // Texture color (background texture)
    vec4 texColor = texture(bgTex, TexCoord);

    // Calculate oscillation for color animation
    float speed = 1.0;
    float intensity = 0.1;
    float oscillation = sin(uTime * speed + uOscillationOffset) * intensity + 0.5;

    // Calculate the final color, combining lighting, texture, and oscillation effect
    vec3 lightingColor = ambient + diffuse + specular;
    vec4 finalColor = texColor * (cubeColour * oscillation);

    // Apply lighting to final color
    FragColor = vec4(finalColor.rgb * lightingColor, 1.0);
}
