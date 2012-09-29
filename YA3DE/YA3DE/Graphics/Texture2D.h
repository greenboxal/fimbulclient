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

#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include <YA3DE/Helpers.h>
#include <YA3DE/Content/ContentManager.h>

namespace YA3DE
{
	namespace Graphics
	{
		class Texture2D : public YA3DE::Content::Resource
		{
		public:
			Texture2D(int textureID);
			Texture2D(int width, int height, int pixelformat, int internalpixelformat, int type);
			~Texture2D();
			
			void SetMinMagFilter(int min, int mag);
			void SetSTWrap(int s, int t);
			void SetEnv(int env);

			void SetData(void *data);

			void Bind(int index = -1);

			DEFPROP_RO(public, unsigned int, TextureID);
			DEFPROP_RO(public, int, Width);
			DEFPROP_RO(public, int, Height);
			DEFPROP_RO(public, int, PixelFormat);
			DEFPROP_RO(public, int, InternalPixelFormat);
			DEFPROP_RO(public, int, Type);
		};
		typedef std::shared_ptr<Texture2D> Texture2DPtr;
	}
}

#endif
