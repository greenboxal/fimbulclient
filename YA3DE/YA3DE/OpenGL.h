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

#ifndef _OPENGL_H_
#define _OPENGL_H_

#define GLEW_MX
#define glewGetContext() (&YA3DE_CurrentGlew)

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#define wglewGetContext() (&YA3DE_CurrentWGlew)
#elif defined(_LINUX)
#define glxewContextInit() (&YA3DE_CurrentXGlew)
#include <GL/glxew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _MSC_VER
extern __declspec(thread) GLEWContext YA3DE_CurrentGlew;
#elif defined(_LINUX)
extern __thread GLEWContext YA3DE_CurrentGlew;
#endif

#ifdef _WIN32
extern __declspec(thread) WGLEWContext YA3DE_CurrentWGlew;
#elif defined(_LINUX)
extern __thread GLXEWContext YA3DE_CurrentXGlew;
#endif

#endif
