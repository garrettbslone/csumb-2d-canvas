#version 330

in vec4 vPos;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTex;

void main() {
        FragColor = texture(uTex, vTexCoord);
}
