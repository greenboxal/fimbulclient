#version 330

#if defined(VERTEX_SHADER)
#define param out
#elif defined(FRAGMENT_SHADER)
#define param in
#else
#error "This shader can only be used as a vertex or fragment."
#endif

// Control
//#define DEBUG_NORMALS

// WorldViewProjection matrix
uniform mat4 WorldViewProjection;

// Light information
uniform vec3 LightPosition;
uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;

// Texture & Transparency information
uniform sampler2D InTexture;
uniform float Alpha;

// Vertex -> Fragment parameters
param vec2 TexCoord0;

#if defined(VERTEX_SHADER)

// Vertex Data
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;

void main()
{
#ifdef DEBUG_NORMALS
	LightColorFactor = VertexNormal;
#else
	TexCoord0 = VertexTexCoord;
#endif
	
	gl_Position = WorldViewProjection * vec4(VertexPosition, 1);
}

#elif defined(FRAGMENT_SHADER)

// Fragment Color
out vec4 OutFragColor;

void main()
{
#ifdef DEBUG_NORMALS
	OutFragColor = vec4(LightColorFactor, 1);
#else
	vec4 color = texture(InTexture, TexCoord0);

	color.a *= Alpha;
	
	if (color.a == 0)
		discard;
	
	OutFragColor = color;
#endif
}

#endif
