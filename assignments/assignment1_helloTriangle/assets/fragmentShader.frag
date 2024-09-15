#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;

in vec4 Colour;

uniform float uTime;

void main()
{
    FragColor = Colour * (sin(uTime) * 0.5 + 0.5);
}