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

#ifndef YA3DE_TEXTURE2D_H
#define YA3DE_TEXTURE2D_H

#include <YA3DE/Helpers.h>
#include <YA3DE/Content/Resource.h>

namespace YADE
{
	class Texture2D : public Resource
	{
	public:
		Texture2D();
		Texture2D(int width, int height, int pixelformat, int internalpixelformat, int type);
		~Texture2D();

		void Assign(unsigned int textureID);
			
		void SetMinMagFilter(int min, int mag);
		void SetSTWrap(int s, int t);

		void SetData(void *data);

		void Bind(int index = -1);
			
		static void InvalidateLastUsedCache()
		{
			_LastUsedTextureUnit = -1;
			_LastUsedTexture = -1;
		}

		DEFPROP_RO(public, unsigned int, TextureID);
		DEFPROP_RO(public, int, Width);
		DEFPROP_RO(public, int, Height);
		DEFPROP_RO(public, int, PixelFormat);
		DEFPROP_RO(public, int, InternalPixelFormat);
		DEFPROP_RO(public, int, Type);

	private:
		static int _LastUsedTextureUnit;
		static int _LastUsedTexture;
			
		int _PAMinFilter;
		int _PAMagFilter;
		int _PASWrap;
		int _PATWrap;
	};
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
}

#endif
