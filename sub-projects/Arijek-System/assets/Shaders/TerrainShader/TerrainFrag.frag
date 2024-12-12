#version 330 core

in vec3 FragPos;  // World-space position from vertex shader
out vec4 FragColor;

uniform vec3 lightDir;            // Directional light direction
uniform vec3 lightColor;          // Light color
uniform vec3 fogColor;            // Fog color
uniform float fogStart;           // Fog start distance
uniform float fogEnd;             // Fog end distance
uniform vec3 cameraPos;           // Camera position in world space
uniform vec3 cameraDir;           // Camera forward direction

uniform float snowHeight;         // Height at which snow starts

void main()
{
    // --- Base Terrain Colors ---
    vec3 snowColor = vec3(1.0, 1.0, 1.0);  // White for snow
    vec3 iceColor = vec3(0.6, 0.8, 1.0);   // Light blue for ice

    // --- Lighting ---
    vec3 normal = normalize(cross(dFdx(FragPos), dFdy(FragPos))); // Flat shading normal
    float diff = max(dot(normal, -lightDir), 0.0);                // Diffuse lighting intensity

    // --- Determine Terrain Layer ---
    vec3 terrainColor;
    if (FragPos.y > snowHeight) {
        terrainColor = snowColor; // Snow
    } else {
        terrainColor = iceColor; // Ice
    }

    // Apply lighting
    terrainColor *= lightColor * diff;

    // --- Fog Effect ---
    // Distance from the camera plane
    float distance = abs(dot(cameraDir, FragPos - cameraPos));
    float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);
    vec3 finalColor = mix(fogColor, terrainColor, fogFactor); // Blend fog and terrain colors

    FragColor = vec4(finalColor, 1.0); // Output final color
}
