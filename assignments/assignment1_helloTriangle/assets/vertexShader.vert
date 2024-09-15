#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;

out vec4 Colour; // Varying

uniform float uTime;

void main()
{
    Colour = aColour; // Pass-through
    vec3 pos = aPos;
    pos.y += (sin(uTime) * 5.0 + pos.x) / 4.0;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
