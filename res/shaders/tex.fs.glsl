#version 330

in vec4 vPos;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTex;
uniform float uAlpha;

void main() {
        FragColor = texture(uTex, vTexCoord);

        if (uAlpha >= 0.f && uAlpha <= 1.f) {
                FragColor.w = uAlpha;
        }
}
