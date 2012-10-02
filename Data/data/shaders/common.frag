#version 330

in vec2 TexCoord0;

uniform sampler2D InTexture;
uniform float Alpha;

out vec4 OutFragColor;

void main()
{
	vec4 color = texture(InTexture, TexCoord0);

	color.a *= Alpha;
	
	OutFragColor = color;
}
