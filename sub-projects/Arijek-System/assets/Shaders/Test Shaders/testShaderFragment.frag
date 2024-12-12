#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// Include Lighting Parameters (from light_params.txt)
//~LightingParams

void main()
{
    // Normalize normal vector
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Compute ambient color
    vec3 ambient = AmbientK * lightColor * lightIntensity;

    // Initialize variables for diffuse and specular components
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Include Lighting Model (from phong.txt or blinn_phong.txt)
    //~LightingModel

    // Calculate lighting based on light type (from directional.txt, point.txt, or spotlight.txt)
    //~LightingType

    // Calculate the final color
    vec3 resultColor = ambient + diffuse + specular;
    resultColor *= (useMaterialColor == 1) ? materialColor : vec3(1.0);

    FragColor = vec4(resultColor, 1.0);
}
