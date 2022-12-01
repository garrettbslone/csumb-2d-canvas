#version 330 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTex;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTex, vTexCoord).r);
//    color = vec4(textColor, 1.0) * sampled;
    FragColor = sampled;
}