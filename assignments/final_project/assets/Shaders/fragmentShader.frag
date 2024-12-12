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
    // Normalize the normal vector
    vec3 norm = normalize(Normal);

    // Ambient component
    vec3 ambient = AmbientK * lightColor * lightIntensity;

    // Diffuse and Specular components initialization
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Define light direction and attenuation for each light type
    vec3 lightDirection;
    float attenuation = 1.0;

    if (lightType == 0) { // Directional Light
                          lightDirection = normalize(-lightDir);
    }
    else if (lightType == 1) { // Point Light
                               lightDirection = normalize(lightPos - FragPos);
                               float distance = length(lightPos - FragPos);
                               attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    }
    else if (lightType == 2) { // Spotlight
                               lightDirection = normalize(lightPos - FragPos);
                               float theta = dot(lightDirection, normalize(-lightDir));

                               if (theta > cutoff) {
                                   float distance = length(lightPos - FragPos);
                                   attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
                               } else {
                                   attenuation = 0.0; // Outside the spotlight cone
                               }
    }

    // Diffuse calculation
    float diffStrength = max(dot(norm, lightDirection), 0.0);
    diffuse = DiffuseK * diffStrength * lightColor * attenuation * lightIntensity;

    // Specular calculation
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;

    if (lightingModel == 0) { // Phong Model
                              vec3 reflectDir = reflect(-lightDirection, norm);
                              spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    }
    else if (lightingModel == 1) { // Blinn-Phong Model
                                   vec3 halfwayDir = normalize(lightDirection + viewDir);
                                   spec = pow(max(dot(norm, halfwayDir), 0.0), Shininess);
    }

    specular = SpecularK * spec * lightColor * attenuation * lightIntensity;

    // Texture color (background texture)
    vec4 texColor = texture(bgTex, TexCoord);

    // Calculate oscillation for color animation
    float speed = 1.0;
    float intensity = 0.1;
    float oscillation = sin(uTime * speed + uOscillationOffset) * intensity + 0.5;

    // Final color combining lighting, texture, and oscillation effect
    vec3 lightingColor = ambient + diffuse + specular;
    vec4 finalColor = texColor * (cubeColour * oscillation);

    // Apply lighting to the final color
    FragColor = vec4(finalColor.rgb * lightingColor, 1.0);
}
