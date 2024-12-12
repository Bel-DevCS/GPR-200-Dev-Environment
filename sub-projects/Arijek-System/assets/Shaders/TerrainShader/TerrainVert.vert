#version 330 core

layout (location = 0) in vec3 aPos;  // Vertex position
layout (location = 1) in vec2 aTexCoord; // Texture coordinates (optional)

out vec3 FragPos;  // Position passed to fragment shader
out vec2 TexCoord; // Texture coordinates passed to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // Transform position to world space
    TexCoord = aTexCoord;                   // Pass texture coordinates
    gl_Position = projection * view * vec4(FragPos, 1.0); // Transform to clip space
}
