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

#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/Graphics/Texture2D.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>

using namespace YADE;

int Texture2D::_LastUsedTextureUnit = -1;
int Texture2D::_LastUsedTexture = -1;

Texture2D::Texture2D()
{
	_TextureID = -1;
	_PAMinFilter = -1;
	_PAMagFilter = -1;
	_PASWrap = -1;
	_PATWrap = -1;
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

void Texture2D::Assign(unsigned int textureID)
{
	_TextureID = textureID;

	if (_TextureID != -1)
	{
		Bind();
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_Width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_Height);

		if (_PAMinFilter != -1 && _PAMagFilter != -1)
			SetMinMagFilter(_PAMinFilter, _PAMagFilter);

		if (_PASWrap != -1 && _PATWrap != -1)
			SetSTWrap(_PASWrap, _PATWrap);
	}
}

void Texture2D::SetMinMagFilter(int min, int mag)
{
	if (_TextureID == -1)
	{
		_PAMinFilter = min;
		_PAMagFilter = mag;
		return;
	}

	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture2D::SetSTWrap(int s, int t)
{
	if (_TextureID == -1)
	{
		_PASWrap = s;
		_PATWrap = t;
		return;
	}

	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture2D::SetData(void *data)
{
	if (_TextureID == -1)
		return;

	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, _InternalPixelFormat, _Width, _Height, 0, _PixelFormat, _Type, data);
}

void Texture2D::Bind(int index)
{
	if (_TextureID == -1)
		return;

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
std::shared_ptr<Texture2D> ContentManager::LoadNew(const std::string &name, bool async)
{
	auto namecopy = name;
	Texture2DPtr tex = std::make_shared<Texture2D>();

	auto main = [=]()
	{
		FilePtr fp = FileManager::Instance()->OpenFile(namecopy);

		if (!fp)
		{
			LOG("Failed loading '%s': file not found", namecopy.c_str());
			return;
		}
	
		int size = fp->GetSize();
		char *data = new char[size];
		fp->Read(data, size);
		fp->Close();

		int texID = SOIL_load_OGL_texture_from_memory((const unsigned char *)data, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_FUCSIA_TRANSPARENCY);

		delete[] data;

		if (texID == -1)
		{
			LOG("Error loading '%s': %s", namecopy.c_str(), SOIL_last_result());
			return;
		}

		tex->Assign(texID);
	
		LOG("Loaded %s %d %x", namecopy.c_str(), texID, std::this_thread::get_id().hash());
	};

	if (async)
		ContentManager::Instance()->Dispatcher.EnqueueAsync(main);
	else
		main();

	ContentManager::Instance()->CacheObject(name, tex);

	return tex;
}
