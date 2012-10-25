#version 330

#if defined(VERTEX_SHADER)
#define param out
#elif defined(FRAGMENT_SHADER)
#define param in
#else
#error "This shader can only be used as a vertex or fragment."
#endif

// WorldViewProjection matrix
uniform mat4 WorldViewProjection;

// Texture & Color information
uniform sampler2D InTexture;
uniform vec3 InColor;

// Vertex -> Fragment parameters
param vec2 AtlasPosition;

#if defined(VERTEX_SHADER)

// Vertex Data
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 VertexTexCoord;

void main()
{
	AtlasPosition = VertexTexCoord;
	
	gl_Position = WorldViewProjection * vec4(VertexPosition, 1);
}

#elif defined(FRAGMENT_SHADER)

// Fragment Color
out vec4 OutFragColor;

void main()
{
	vec4 color = texture(InTexture, AtlasPosition);

	color.rgb *= InColor;
	
	OutFragColor = color;
}

#endif
