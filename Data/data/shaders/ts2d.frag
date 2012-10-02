#version 330

in vec2 TexCoord0;

uniform sampler2D InTexture;
uniform vec4 InColor;

out vec4 OutFragColor;

void main()
{
	OutFragColor = texture(InTexture, TexCoord0) * InColor;
}
