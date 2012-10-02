#version 330

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;

uniform mat4 WorldViewProjection;

out vec2 TexCoord0;

void main()
{
	TexCoord0 = VertexTexCoord;
	
	gl_Position = WorldViewProjection * vec4(VertexPosition, 1);
}
