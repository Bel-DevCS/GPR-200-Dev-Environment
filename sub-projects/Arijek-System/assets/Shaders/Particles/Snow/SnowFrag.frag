#version 330 core
out vec4 FragColor;

uniform vec4 color; // Particle color

void main()
{
    FragColor = color;
}
