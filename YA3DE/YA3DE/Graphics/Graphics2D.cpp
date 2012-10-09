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
#include <YA3DE/Content/StringResource.h>
#include <YA3DE/Graphics/Graphics2D.h>
#include <YA3DE/Graphics/ShaderProgram.h>
#include <YA3DE/Graphics/VertexBuffer.h>
#include <YA3DE/Graphics/IndexBuffer.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace YA3DE;
using namespace YA3DE::Content;
using namespace YA3DE::Graphics;

struct VertexPositionTexture
{
	glm::vec3 Position;
	glm::vec2 TexCoord;

	VertexPositionTexture(glm::vec3 &position, glm::vec2 &texcoord)
    {
        Position = position;
        TexCoord = texcoord;
    }

	static VertexDeclaration VertexDeclaration;
};

static VertexElement VertexPositionTextureElements[] =
{
	VertexElement(VertexElementFormat::Vector3, VertexElementUsage::Position),
	VertexElement(VertexElementFormat::Vector2, VertexElementUsage::TextureCoordinate)
};
VertexDeclaration VertexPositionTexture::VertexDeclaration(VertexPositionTextureElements, 2);

class TextureStep2D : public Graphics2D::Step2D
{
public:
	TextureStep2D(Texture2DPtr texture, glm::uvec4 &rect, glm::uvec4 &srect, glm::vec4 &color, float rotation, float depth)
	{
		if (!_isShaderInit)
			InitShader();

		_texture = texture;
		_color = color;

		_mvp = _2dproj * glm::rotate(glm::mat4(), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

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
	
	void InitShader()
	{
		Game *game = Game::Instance();
		bool success = false;

		_shader = ShaderProgramPtr(new ShaderProgram());

		ShaderPtr vs(new Shader(GL_VERTEX_SHADER));
		success = vs->Load((char *)YA3DE::Content::ContentManager::Instance()->Load<StringResource>("data/shaders/ts2d.vert")->GetData());
		_shader->AddShader(vs);

		ShaderPtr ps(new Shader(GL_FRAGMENT_SHADER));
		success = ps->Load((char *)YA3DE::Content::ContentManager::Instance()->Load<StringResource>("data/shaders/ts2d.frag")->GetData());
		_shader->AddShader(ps);

		success = _shader->Link();

		_2dproj = glm::ortho<float>(0, (const float)game->Window.Size.x, (const float)game->Window.Size.y, 0, 1, -1);

		_isShaderInit = true;
	}

	virtual void Draw()
	{
		_shader->Begin();
		_shader->SetUniform("WVPMatrix", _mvp);
		_shader->SetUniform("InColor", _color);
		_shader->SetUniform("InTexture", 0);

		_texture->Bind(0);
		_vbo->Render(GL_QUADS, _ibo, 4);

		_shader->End();
	}

private:
	Texture2DPtr _texture;
	glm::mat4 _mvp;
	glm::vec4 _color;
	IndexBufferPtr _ibo;
	VertexBufferPtr _vbo;

	static bool _isShaderInit;
	static ShaderProgramPtr _shader;
	static glm::mat4 _2dproj;
};

bool TextureStep2D::_isShaderInit = false;
ShaderProgramPtr TextureStep2D::_shader;
glm::mat4 TextureStep2D::_2dproj;


class FontStep2D : public Graphics2D::Step2D
{
public:
	FontStep2D(FontPtr font, int height, std::string str, glm::uvec2 &pos, glm::vec4 &color, FontStyle style)
	{
		_font = font;
		_height = height;
		_str = str;
		_pos = pos;
		_color = color;
		_style = style;
	}
	
	virtual void Draw()
	{
		//_font->Render(_str, _height, _pos, _color, _style);
	}

private:
	FontPtr _font;
	int _height;
	std::string _str;
	glm::uvec2 _pos;
	glm::vec4 _color;
	FontStyle _style;
};

void Graphics2D::Begin()
{
	_steps.clear();
}

void Graphics2D::Draw(Texture2DPtr texture, glm::uvec4 &rect, glm::uvec4 &srect, glm::vec4 &color, float rotation, float depth)
{
	glm::uvec4 nrect = rect;

	nrect.x += _bounds.x;
	nrect.y += _bounds.y;

	_steps.push_back(new TextureStep2D(texture, nrect, srect, color, rotation, depth));
}

void Graphics2D::Draw(FontPtr font, int height, std::string str, glm::uvec2 &pos, glm::vec4 &color, FontStyle style)
{
	glm::uvec2 npos = pos;

	npos.x += _bounds.x;
	npos.y += _bounds.y;

	_steps.push_back(new FontStep2D(font, height, str, npos, color, style));
}

void Graphics2D::End()
{
	std::list<Step2D *>::iterator it;
	for (it = _steps.begin(); it != _steps.end(); it++)
	{
		(*it)->Draw();
		delete *it;
	}

	_steps.clear();
}
