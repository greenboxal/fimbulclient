/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#include <YA3DE/OpenGL.h>
#include <YA3DE/Graphics/Texture2D.h>
#include <YA3DE/FileSystem/FileManager.h>
#include <SOIL/SOIL.h>

using namespace YA3DE;
using namespace YA3DE::Content;
using namespace YA3DE::Graphics;
using namespace YA3DE::FileSystem;

int Texture2D::_LastUsedTextureUnit = -1;
int Texture2D::_LastUsedTexture = -1;

Texture2D::Texture2D(int textureID)
{
	_TextureID = textureID;

	Bind();
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_Width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_Height);
}

Texture2D::Texture2D(int width, int height, int pixelformat, int internalpixelformat, int type)
{
	glGenTextures(1, &_TextureID);
	_Width = width;
	_Height = height;
	_PixelFormat = pixelformat;
	_InternalPixelFormat = internalpixelformat;
	_Type = type;
}

Texture2D::~Texture2D()
{
	if (_LastUsedTexture == _TextureID)
		_LastUsedTexture = -1;

	glDeleteTextures(1, &_TextureID);
}

void Texture2D::SetMinMagFilter(int min, int mag)
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture2D::SetSTWrap(int s, int t)
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture2D::SetData(void *data)
{
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, _InternalPixelFormat, _Width, _Height, 0, _PixelFormat, _Type, data);
}

void Texture2D::Bind(int index)
{
	if (index != -1 && _LastUsedTextureUnit != index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		_LastUsedTextureUnit = index;
	}

	if (_LastUsedTexture != _TextureID)
	{
		glBindTexture(GL_TEXTURE_2D, _TextureID);
		_LastUsedTexture = _TextureID;
	}
}

template<>
std::shared_ptr<Texture2D> ContentManager::LoadNew(const std::string &name)
{
	FilePtr fp = FileManager::Instance()->OpenFile(name);

	if (!fp)
		return NULL;

	int size = fp->GetSize();
	char *data = new char[size];
	fp->Read(data, size);
	fp->Close();
	
	int texID = SOIL_load_OGL_texture_from_memory((const unsigned char *)data, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_FUCSIA_TRANSPARENCY);
	
	delete[] data;

	if (texID == -1)
		return false;

	std::shared_ptr<Texture2D> tex(new Texture2D(texID));

	ContentManager::Instance()->CacheObject(name, tex);

	return tex;
}
