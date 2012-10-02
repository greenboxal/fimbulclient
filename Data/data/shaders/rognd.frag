#version 330

in vec2 TexCoord0;
in vec2 TexCoord1;

uniform sampler2D InTexture;
uniform sampler2D InLightmap;

out vec4 OutFragColor;

void main()
{
	vec4 color = texture(InTexture, TexCoord0);
	vec4 lmap = texture(InLightmap, TexCoord1);

	color.rgb *= lmap.a;
	color.rgb += lmap.rgb;
	
	OutFragColor = color;
}
