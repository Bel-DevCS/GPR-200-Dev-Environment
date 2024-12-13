#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform vec4 color;

void main() {
    float distance = length(TexCoords - vec2(0.5)); // Distance from center
    float alpha = smoothstep(0.45, 0.5, 1.0 - distance); // Gradient fade
    FragColor = vec4(color.rgb, color.a * alpha); // Apply alpha
}
