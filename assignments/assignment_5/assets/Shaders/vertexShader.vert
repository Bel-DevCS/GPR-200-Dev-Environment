#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 Colour;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;
uniform float uBounce;

void main()
{
    Colour = aColour;
    TexCoord = aTexCoord;

    // Pass the fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Pass the normal, transformed by the normal matrix to maintain correct lighting
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Apply the bounce animation to the position
    vec3 animatedPos = aPos;
    animatedPos.y += sin(uTime + aPos.x * 0.5) * uBounce;

    gl_Position = projection * view * model * vec4(animatedPos, 1.0);
}
