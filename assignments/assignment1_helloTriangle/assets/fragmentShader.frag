#version 330 core
out vec4 FragColor;

in vec4 Colour;

uniform float uTime = 1.0;

void main()
{
    FragColor = Colour * (sin(uTime) * 0.5 + 0.5);
}