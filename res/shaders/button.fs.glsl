#version 330

in vec4 vPos;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform vec2 uCenter;
uniform vec2 uBorderRadius;
uniform vec4 uBorderColor;

void main() {
        vec2 delta = abs(vec2(vPos.xy) - uCenter);

        if (delta.x >= uBorderRadius.x || delta.y >= uBorderRadius.y) {
                FragColor = uBorderColor;
        } else {
                FragColor = vec4(0.f, 0.f, 0.f, 1.f);
        }
}
