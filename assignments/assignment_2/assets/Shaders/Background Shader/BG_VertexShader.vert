#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 Colour; // Pass-through
out vec2 TexCoord;

uniform float uTime;

void main()
{
    // Pass the color through
    Colour = aColour;

    // Position stays the same
    gl_Position = vec4(aPos, 1.0);

    // Scroll the texture coordinates over time
    float scrollSpeed = 0.2; //Scroll Speed
    TexCoord = vec2(aTexCoord.x + uTime * scrollSpeed, aTexCoord.y) /2 ;
}