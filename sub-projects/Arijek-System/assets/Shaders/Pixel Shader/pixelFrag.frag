#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 materialColor;
uniform int useMaterialColor;
uniform int pixelSize;  // Controls pixelation effect

void main()
{
    // Calculate pixelated texture coordinates
    vec2 pixelatedCoords = floor(TexCoords * pixelSize) / pixelSize;

    // Sample the texture using pixelated coordinates
    vec4 texColor = texture(texture_diffuse1, pixelatedCoords);

    // Create a variation in color based on texture coordinates
    vec3 texCoordsColor = vec3(pixelatedCoords, 0.5);

    // Simulate basic shading by darkening the bottom half of the model
    float shading = 0.5 + TexCoords.y * 0.5; // Varies from 0.5 to 1.0
    vec3 shadedColor = materialColor * shading;

    // Blend the material color with the texCoords-based color for variation
    vec3 blendedColor = mix(shadedColor, texCoordsColor, 0.2); // 20% texture-based variation

    // Use material color or texture based on the uniform
    if (useMaterialColor == 1)
    {
        FragColor = vec4(blendedColor, 1.0);
    }
    else
    {
        FragColor = texColor;
    }
}
