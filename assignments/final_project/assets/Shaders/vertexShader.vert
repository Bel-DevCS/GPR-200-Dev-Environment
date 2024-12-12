#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 Colour;
out vec2 TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;
uniform float uBounce;
void main()
{
    Colour = aColour;
    TexCoord = aTexCoord;

    vec3 animatedPos = aPos;
    animatedPos.y += sin(uTime + aPos.x * 0.5) * uBounce;

    gl_Position = projection * view * model * vec4(animatedPos, 1.0);
}
