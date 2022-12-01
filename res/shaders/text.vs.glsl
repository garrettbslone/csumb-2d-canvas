#version 330 core

layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>

out vec2 vTexCoord;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(aVertex.xy, 0.0, 1.0);
    vTexCoord = aVertex.zw;
}