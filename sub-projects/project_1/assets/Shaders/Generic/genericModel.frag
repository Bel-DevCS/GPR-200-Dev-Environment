#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 materialColor;
uniform int useMaterialColor;

void main()
{
    // Create a color based on the texture coordinates for visual variation
    vec3 texCoordsColor = vec3(TexCoords, 0.5); // Add a base value to prevent it from being pure black

    // Blend the material color with the texCoordsColor
    vec3 blendedColor = mix(materialColor, texCoordsColor, 0.1); // Adjust blend factor as needed

    if (useMaterialColor == 1)
    {
        FragColor = vec4(blendedColor, 1.0);
    }
    else
    {
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        FragColor = texColor;
    }
}
