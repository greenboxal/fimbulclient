#version 330

#if defined(VERTEX_SHADER)
#define param out
#elif defined(FRAGMENT_SHADER)
#define param in
#else
#error "This shader can only be used as a vertex or fragment."
#endif

// Texture & Transparency information
uniform sampler2D InTexture;
uniform mat4 Projection;

// Vertex -> Fragment parameters
param vec2 TexCoord0;

#if defined(VERTEX_SHADER)

// Vertex Data
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoord;

void main()
{
	TexCoord0 = VertexTexCoord;
	gl_Position = Projection * vec4(VertexPosition, 1);
}

#elif defined(FRAGMENT_SHADER)

// Fragment Color
out vec4 OutFragColor;

void main()
{
	OutFragColor = texture(InTexture, TexCoord0);;
}

#endif
