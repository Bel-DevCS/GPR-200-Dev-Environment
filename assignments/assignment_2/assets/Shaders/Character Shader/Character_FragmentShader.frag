 #version 330 core
 #extension GL_ARB_separate_shader_objects : enable

    out vec4 FragColor;

    in vec4 Colour;
    in vec2 TexCoord;

    uniform float uTime;
    uniform sampler2D texture1;

    void main()
    {
        vec4 textureColour = texture(texture1, TexCoord);
        if(textureColour.a < 0.4)
            discard;

        FragColor = textureColour;

        //Colour Animation
        float speed = 1.0;
        float intensity = 0.5;
        float oscillation = sin(uTime * speed) * intensity + 0.5;

        //Colour Declaration
        //FragColor = Colour * oscillation;

        //FragColor = texture(texture1, TexCoord);
    }
