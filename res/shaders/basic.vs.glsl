#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vPos;
out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform float zIndex;
uniform mat4 MVP;
uniform float w;
uniform float h;


void main()
{
//	vec4 pos = vec4(aPos.x / w / 2.f - 1.f, aPos.y / h / 2.f - 1.f, 0.f, 1.f);
//	gl_Position = uModel * uView * uProj * pos;
	vec4 pos = MVP * vec4(aPos, zIndex, 1.f);
//	gl_PointSize = 50.f;
	gl_Position = pos;

	vPos = pos;
	vColor = aColor;
	vTexCoord = aTexCoord;
}
