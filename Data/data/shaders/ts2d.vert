#version 330

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoord;

uniform mat4 WVPMatrix;

out vec2 TexCoord0;

void main()
{
	TexCoord0 = VertexTexCoord;
	
	gl_Position = WVPMatrix * vec4(VertexPosition, 1);
}
