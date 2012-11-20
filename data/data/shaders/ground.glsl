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

// ViewProjection matrix as we don't need a world matrix
uniform mat4 ViewProjection;

// Textures
uniform sampler2D InTexture;
uniform sampler2D InLightmap;

// Vertex -> Fragment parameters
param vec2 TexCoord0;
param vec2 TexCoord1;
param vec3 SurfaceVertexColor;

#if defined(VERTEX_SHADER)

// Vertex Data
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;
layout(location = 3) in vec2 VertexLightmap;
layout(location = 4) in vec3 VertexColor;

void main()
{
#ifdef DEBUG_NORMALS
	SurfaceVertexColor = VertexNormal;
#else
	TexCoord0 = VertexTexCoord;
	TexCoord1 = VertexLightmap;
	SurfaceVertexColor = VertexColor;
#endif
	
	gl_Position = ViewProjection * vec4(VertexPosition, 1);
}

#elif defined(FRAGMENT_SHADER)

// Fragment Color
out vec4 OutFragColor;

void main()
{
#ifdef DEBUG_NORMALS
	OutFragColor = vec4(SurfaceVertexColor, 1);
#else
	vec4 color = texture(InTexture, TexCoord0);
	vec4 lmap = texture(InLightmap, TexCoord1);

	color.rgb *= SurfaceVertexColor;
	color.rgb += lmap.rgb;
	color.rgb *= lmap.a;
	
	if (color.a == 0)
		discard;
	
	OutFragColor = color;
#endif
}

#endif
