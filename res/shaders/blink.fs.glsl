#version 330

in vec4 vPos;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform float uTime;
uniform float uRate;

void main() {
        FragColor = vColor;
        FragColor.a *= cos(uTime * uRate);
}
