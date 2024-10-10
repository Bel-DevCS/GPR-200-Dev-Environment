#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;

out vec4 Colour;

uniform mat4 uTransform;  // Transformation matrix from C++ code

void main()
{
    Colour = aColour; // Pass-through

    // Apply transformation to the vertex position
    gl_Position = uTransform * vec4(aPos, 1.0);
}
