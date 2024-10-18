#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 Colour;
out vec2 TexCoord;

//uniform mat4 uTransform;  // Transformation matrix from C++ code

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Colour = aColour; // Pass-through
    TexCoord = aTexCoord;

    // Apply transformation to the vertex position
    //gl_Position = uTransform * vec4(aPos, 1.0);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
