#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 Colour; // Varying
out vec2 TexCoord;

uniform float uTime;

void main()
{
    //Initial Declarations
    Colour = aColour; // Pass-through
    vec3 pos = aPos;

    //Position Animation
    float bounce = sin(uTime * 2.0) * 0.1;
  //  pos.y += bounce;

    //Position Declaration
    gl_Position = vec4(pos, 1.0);

    //Texture Declaration?
    TexCoord = aTexCoord;
}
