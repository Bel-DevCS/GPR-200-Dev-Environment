#version 330 core
out vec4 FragColor;

uniform vec3 cubeColor; // Color for the light cube, ideally same as light color

void main()
{
    FragColor = vec4(cubeColor, 1.0); // Display as a solid color
}
