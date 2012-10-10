/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	YA3DE is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#include <YA3DE/Game.h>
#include <YA3DE/Graphics/Graphics2D.h>
#include <YA3DE/Graphics/IndexBuffer.h>
#include <YA3DE/Graphics/VertexBuffer.h>
#include <YA3DE/Graphics/ShaderProgram.h>
#include <YA3DE/Graphics/VertexPositionTexture.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace YA3DE;
using namespace YA3DE::Content;
using namespace YA3DE::Graphics;

static VertexElement VertexPositionTextureElements[] =
{
	VertexElement(VertexElementFormat::Vector3, VertexElementUsage::Position),
	VertexElement(VertexElementFormat::Vector2, VertexElementUsage::TextureCoordinate)
};
VertexDeclaration VertexPositionTexture::VertexDeclaration(VertexPositionTextureElements, 2);

class TextureStep2D : public Graphics2D::Step2D
{
public:
	TextureStep2D(Texture2DPtr texture, const glm::uvec4 &orect, const glm::uvec4 &srect, const glm::vec4 &color, const float rotation, const float depth)
	{
		glm::uvec4 rect = orect;

		_texture = texture;
		_color = color;

		_mvp = glm::rotate(glm::ortho<float>(0, (const float)Game::Instance()->Window.Size.x, (const float)Game::Instance()->Window.Size.y, 0, 1, -1), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		if (rect.z == 0)
			rect.z = srect.z;

		if (rect.w == 0)
			rect.w = srect.w;

		VertexPositionTexture vertices[] =
		{
			VertexPositionTexture(glm::vec3(rect.x, rect.y, depth), glm::vec2(0, 0)),
			VertexPositionTexture(glm::vec3(rect.x + rect.z, rect.y, depth), glm::vec2(1, 0)),
			VertexPositionTexture(glm::vec3(rect.x + rect.z, rect.y + rect.w, depth), glm::vec2(1, 1)),
			VertexPositionTexture(glm::vec3(rect.x, rect.y + rect.w, depth), glm::vec2(0, 1)),
		};

		unsigned short indices[] =
		{
			0, 1, 2, 3
		};

		_vbo = std::make_shared<VertexBuffer>(VertexPositionTexture::VertexDeclaration);
		_vbo->SetData(vertices, 4, GL_STATIC_DRAW);

		_ibo = std::make_shared<IndexBuffer>(GL_UNSIGNED_SHORT);
		_ibo->SetData(indices, 4, GL_STATIC_DRAW);
	}

	virtual void Draw()
	{
		_shader->Begin();
		_shader->SetUniform("WorldViewProjection", _mvp);
		_shader->SetUniform("InColor", _color);
		_shader->SetUniform("InTexture", 0);

		_texture->Bind(0);
		_vbo->Render(GL_QUADS, _ibo, 4);

		_shader->End();
	}

private:
	glm::mat4 _mvp;
	glm::vec4 _color;
	
	Texture2DPtr _texture;
	IndexBufferPtr _ibo;
	VertexBufferPtr _vbo;
	ShaderProgramPtr _shader;
};

class FontStep2D : public Graphics2D::Step2D
{
public:
	FontStep2D(FontPtr font, const std::string &str, const glm::uvec2 &pos, const glm::vec4 &color)
	{
		_font = font;
		_str = str;
		_pos = pos;
		_color = color;
	}
	
	virtual void Draw()
	{
		_font->Render(_str, _pos, _color);
	}

private:
	FontPtr _font;
	std::string _str;
	glm::uvec2 _pos;
	glm::vec4 _color;
};

void Graphics2D::Begin()
{
	_steps.clear();
}

void Graphics2D::Draw(Texture2DPtr texture, const glm::uvec4 &rect, const glm::uvec4 &srect, const glm::vec4 &color, const float rotation, const float depth)
{
	glm::uvec4 nrect = rect;

	nrect.x += _bounds.x;
	nrect.y += _bounds.y;

	_steps.push_back(new TextureStep2D(texture, nrect, srect, color, rotation, depth));
}

void Graphics2D::Draw(FontPtr font, const std::string &str, const glm::uvec2 &pos, const glm::vec4 &color)
{
	glm::uvec2 npos = pos;

	npos.x += _bounds.x;
	npos.y += _bounds.y;

	_steps.push_back(new FontStep2D(font, str, npos, color));
}

void Graphics2D::End()
{
	for (Step2D *step : _steps)
	{
		step->Draw();
		delete step;
	}

	_steps.clear();
}
