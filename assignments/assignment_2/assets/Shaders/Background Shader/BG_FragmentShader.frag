#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;

in vec4 Colour;
in vec2 TexCoord;

uniform sampler2D texture1; // The background texture

void main()
{

    vec4 textureColour = texture(texture1, TexCoord);
    FragColor = textureColour * Colour;
}