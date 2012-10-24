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

#pragma once

namespace YA3DE
{
	namespace System
	{
		struct VideoMode
		{
			VideoMode()
			{
				Width = 800;
				Height = 600;
				BitsPerPixel = 32;
				FullScreen = false;
				AASamples = 0;
				GLMajor = 3;
				GLMinor = 1;
			}

			VideoMode(int width, int height)
			{
				Width = width;
				Height = height;
				BitsPerPixel = 32;
				FullScreen = false;
				AASamples = 0;
				GLMajor = 3;
				GLMinor = 1;
			}

			VideoMode(int width, int height, int bpp)
			{
				Width = width;
				Height = height;
				BitsPerPixel = bpp;
				FullScreen = false;
				AASamples = 0;
				GLMajor = 3;
				GLMinor = 1;
			}

			VideoMode(int width, int height, int bpp, bool fullscreen)
			{
				Width = width;
				Height = height;
				BitsPerPixel = bpp;
				FullScreen = fullscreen;
				AASamples = 0;
				GLMajor = 3;
				GLMinor = 1;
			}

			VideoMode(int width, int height, int bpp, bool fullscreen, int aasamples)
			{
				Width = width;
				Height = height;
				BitsPerPixel = bpp;
				FullScreen = fullscreen;
				AASamples = aasamples;
				GLMajor = 3;
				GLMinor = 1;
			}
			
			VideoMode(int width, int height, int bpp, bool fullscreen, int aasamples, int glmajor, int glminor)
			{
				Width = width;
				Height = height;
				BitsPerPixel = bpp;
				FullScreen = fullscreen;
				AASamples = aasamples;
				GLMajor = glmajor;
				GLMinor = glminor;
			}

			int Width;
			int Height;
			int BitsPerPixel;
			bool FullScreen;
			int AASamples;
			int GLMajor;
			int GLMinor;
		};
	}
}

