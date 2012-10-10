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
#include <YA3DE/Math.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/Graphics/Font.h>
#include <YA3DE/Graphics/Graphics2D.h>
#include <YA3DE/Graphics/IndexBuffer.h>
#include <YA3DE/Graphics/VertexBuffer.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace YA3DE;
using namespace YA3DE::Content;
using namespace YA3DE::Graphics;
using namespace YA3DE::FileSystem;

static FT_Library FreeType = NULL;

Font::Font(void *memory, unsigned int mem_size, int size, int style, FontLoadPolicy policy)
{
	_Size = size;
	_Style = style;
	_Glyphs.resize(256);

	if (FreeType == NULL)
		FT_Init_FreeType(&FreeType);

	FT_New_Memory_Face(FreeType, (const FT_Byte *)memory, mem_size, 0, &_Face);
	FT_Set_Char_Size(_Face, size << 6, size << 6, 0, 0);
	
	_Height = _Face->height >> 6;
	_Texture = -1;
	_AtlasWidth = 0;

	int mSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mSize);

	int x = 0, y = 0, topHeight = 0;
	for (int i = 0x20; i <= 0xff; i++)
	{
		int id = FT_Get_Char_Index(_Face, (FT_ULong)i);

		if (FT_Load_Glyph(_Face, id, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER))
			continue;

		topHeight = std::max(topHeight, _Face->glyph->bitmap.rows);

		if (x + _Face->glyph->bitmap.width + 1 > mSize)
		{
			_AtlasWidth = std::max(x, _AtlasWidth);

			y++;
			x = 0;
		}

		_Glyphs[i].AtlasPosition = glm::uvec2(x, y);
		x += _Face->glyph->bitmap.width + 1;
	}

	if (x > 0)
	{
		_AtlasWidth = std::max(x, _AtlasWidth);
		y++;
	}

	for (int i = 0x20; i <= 0xff; i++)
		_Glyphs[i].AtlasPosition.y *= topHeight + 1;

	_AtlasHeight = y * (topHeight + 1);

	_Atlas = new unsigned char[_AtlasWidth * _AtlasHeight];
	memset(_Atlas, 0, _AtlasWidth * _AtlasHeight);
	
	glGenTextures(1, &_Texture);
	glBindTexture(GL_TEXTURE_2D, _Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (policy == FontLoadPolicy::Direct)
	{
		for (int i = 0x20; i <= 0xff; i++)
			GetGlyph(i);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _AtlasWidth, _AtlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, _Atlas);
	}
	else
		throw NULL;

	_Shader = ContentManager::Instance()->Load<ShaderProgram>("data/shaders/font.glsl");

	Texture2D::InvalidateLastUsedCache();
}

Font::~Font()
{
	if (_Face)
		FT_Done_Face(_Face);

	if (_Texture != -1)
		glDeleteTextures(1, &_Texture);

	delete _Memory;
}

const FontGlyph *Font::GetGlyph(char c)
{
	if (_Glyphs[(unsigned char)c].Cached)
		return &_Glyphs[(unsigned char)c];

	if (_Face == NULL)
		return NULL;

	FontGlyph *g = &_Glyphs[(unsigned char)c];
	g->GlyphID = FT_Get_Char_Index(_Face, (FT_ULong)(unsigned char)c);

	if (FT_Load_Glyph(_Face, g->GlyphID, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER))
		return NULL;

	FT_Bitmap *bitmap = &_Face->glyph->bitmap;

	int x = _Face->glyph->bitmap_left;
	int y = _Height - _Face->glyph->bitmap_top;
	int w = bitmap->width;
	int h = bitmap->rows;

	LOG("%c - %d.%d %dx%x", c, x, y, w, h);

	SetAtlasRegion(g->AtlasPosition.x, g->AtlasPosition.y, w, h, bitmap->buffer, bitmap->pitch);
	
	if (FT_Load_Glyph(_Face, g->GlyphID, FT_LOAD_NO_HINTING | FT_LOAD_RENDER))
		return NULL;

	float u[2];
	float v[2];

	u[0] = g->AtlasPosition.x / (float)_AtlasWidth;
	u[1] = (g->AtlasPosition.x + w) / (float)_AtlasWidth;

	v[0] = g->AtlasPosition.y / (float)_AtlasHeight;
	v[1] = (g->AtlasPosition.y + h) / (float)_AtlasHeight;

	g->Vertices[0] = VertexPositionTexture(glm::vec3(x, y, 0), glm::vec2(u[0], v[0]));
	g->Vertices[1] = VertexPositionTexture(glm::vec3(x + w, y, 0), glm::vec2(u[1], v[0]));
	g->Vertices[2] = VertexPositionTexture(glm::vec3(x, y + h, 0), glm::vec2(u[0], v[1]));
	g->Vertices[3] = VertexPositionTexture(glm::vec3(x + w, y + h, 0), glm::vec2(u[1], v[1]));
	
	g->Metrics = _Face->glyph->metrics;
	g->Advance = _Face->glyph->advance;
	g->Cached = true;

	return g;
}

void Font::SetAtlasRegion(int x, int y, int w, int h, unsigned char *data, int pitch)
{
	assert(x + w <= _AtlasWidth);
	assert(y + h <= _AtlasHeight);

	for (int i = 0; i < h; i++)
		memcpy(_Atlas + (y + i) * _AtlasWidth + x, data + (i * pitch), w);
}

int Font::GetAdvance(char c, char n)
{
	if (n && FT_HAS_KERNING(_Face))
	{
		FT_Vector kern;

		FT_Get_Kerning(_Face, c, n, FT_KERNING_DEFAULT, &kern);

		return (kern.x >> 6) + (GetGlyph(c)->Advance.x >> 6);
	}
	else
	{
		return GetGlyph(c)->Advance.x >> 6;
	}
}

void Font::Render(const std::string &text, const glm::uvec2 &pos, const glm::vec4 &color)
{
	glBindTexture(GL_TEXTURE_2D, _Texture);

	std::vector<VertexPositionTexture> vertexdata;
	vertexdata.resize(text.size() * 4);

	std::vector<unsigned short> indicedata;
	indicedata.resize(text.size() * 6);

	int vertexcount = 0, indicecount = 0;
	float advance = 0;
	for (unsigned int i = 0; i < text.size(); i++)
	{
		char next = text[i] + 1;

		if (text[i] < 0x20)
			continue;

		if (next < 0x20)
			next = 0;

		int start = vertexcount;
		vertexcount += 4;

		const FontGlyph *g = GetGlyph(text[i]);

		if (g == NULL)
			continue;

		vertexdata[start + 0] = g->Vertices[0];
		vertexdata[start + 1] = g->Vertices[1];
		vertexdata[start + 2] = g->Vertices[2];
		vertexdata[start + 3] = g->Vertices[3];

		vertexdata[start + 0].Position.x += advance;
		vertexdata[start + 1].Position.x += advance;
		vertexdata[start + 2].Position.x += advance;
		vertexdata[start + 3].Position.x += advance;

		indicedata[indicecount + 0] = start + 0;
		indicedata[indicecount + 1] = start + 1;
		indicedata[indicecount + 2] = start + 2;
		indicedata[indicecount + 3] = start + 1;
		indicedata[indicecount + 4] = start + 2;
		indicedata[indicecount + 5] = start + 3;
		indicecount += 6;

		advance += GetAdvance(text[i], next);
	}

	VertexBufferPtr vbo = std::make_shared<VertexBuffer>(VertexPositionTexture::VertexDeclaration);
	vbo->SetData(&vertexdata[0], vertexcount, GL_STATIC_DRAW);

	IndexBufferPtr ibo = std::make_shared<IndexBuffer>(GL_UNSIGNED_SHORT);
	ibo->SetData(&indicedata[0], indicecount, GL_STATIC_DRAW);

	_Shader->Begin();
	_Shader->SetUniform("InTexture", 0);
	_Shader->SetUniform("InColor", glm::vec3(color.rgb));
	_Shader->SetUniform("WorldViewProjection", glm::translate(glm::ortho<float>(0, (const float)Game::Instance()->Window.Size.x, (const float)Game::Instance()->Window.Size.y, 0, 1, -1), glm::vec3(pos, 0)));
	vbo->Render(GL_TRIANGLES, ibo, vertexcount);
	_Shader->End();

	Texture2D::InvalidateLastUsedCache();
}

int Font::Measure(const std::string &text)
{
	int size = 0;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		char next = text[i] + 1;

		if (text[i] < 0x20)
			continue;

		if (next < 0x20)
			next = 0;

		size += (int)GetAdvance(text[i], next);
	}

	return size;
}
