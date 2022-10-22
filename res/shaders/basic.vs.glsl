#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vPos;
out vec4 vColor;
out vec2 vTexCoord;

uniform float zIndex;
uniform mat4 MVP;
uniform float w;
uniform float h;


void main()
{
	vec4 pos = MVP * vec4(aPos, zIndex, 1.f);
	gl_Position = pos;

	vPos = pos;
	vColor = aColor;
	vTexCoord = aTexCoord;
}
