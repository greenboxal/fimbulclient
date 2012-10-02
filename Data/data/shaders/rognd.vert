#version 330

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;
layout(location = 3) in vec2 VertexLightmap;

uniform mat4 ViewProjection;

out vec2 TexCoord0;
out vec2 TexCoord1;

void main()
{
	TexCoord0 = VertexTexCoord;
	TexCoord1 = VertexLightmap;
	
	gl_Position = ViewProjection * vec4(VertexPosition, 1);
}
