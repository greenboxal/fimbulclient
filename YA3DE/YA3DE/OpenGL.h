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

#define GLEW_MX
#define glewGetContext() (&YA3DE_CurrentGlew)
#define wglewGetContext() (&YA3DE_CurrentWGlew)

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

#include <GL/GL.h>
#include <GL/GLU.h>

extern __declspec(thread) GLEWContext YA3DE_CurrentGlew;

#ifdef _WIN32
extern __declspec(thread) WGLEWContext YA3DE_CurrentWGlew;
#endif

