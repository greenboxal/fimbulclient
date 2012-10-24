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

#include <stdafx.h>

#ifdef _WIN32

#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/System/GLContext.h>
#include <YA3DE/System/RenderWindow.h>

using namespace YA3DE::System;

static std::mutex _GLGuard;
__declspec(thread) GLEWContext YA3DE_CurrentGlew;
__declspec(thread) WGLEWContext YA3DE_CurrentWGlew;

GLContext::GLContext(RenderWindow *owner)
{
	_Window = owner;
	_DeviceContext = GetDC(owner->NativeHandle);
}

GLContext::~GLContext()
{
	if (_NativeHandle != NULL)
		Destroy();
}

bool GLContext::Create(GLContext *share)
{
	VideoMode &vm = _Window->Mode;

	if (_DeviceContext == NULL)
		return false;

	if (share == NULL)
	{
		int bestFormat = 0;
		int origAA = vm.AASamples;

		if (vm.AASamples > 0)
		{
			PFNWGLCHOOSEPIXELFORMATARBPROC cpfarb = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
			if (cpfarb != NULL)
			{
				int intAttributes[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
					WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_SAMPLES_ARB,        vm.AASamples,
					0,
				};

				int formats[128];
				UINT nbFormats;
				float floatAttributes[] = {0, 0};
				bool isValid = cpfarb(_DeviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
				while ((!isValid || (nbFormats == 0)) && vm.AASamples > 0)
				{
					vm.AASamples--;
					intAttributes[11] = vm.AASamples;
					isValid = cpfarb(_DeviceContext, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
				}

				if (isValid && (nbFormats > 0))
				{
					int bestScore = 0xFFFF;

					for (UINT i = 0; i < nbFormats; ++i)
					{
						PIXELFORMATDESCRIPTOR attributes;
						attributes.nSize    = sizeof(attributes);
						attributes.nVersion = 1;
						DescribePixelFormat(_DeviceContext, formats[i], sizeof(attributes), &attributes);

						int color = attributes.cRedBits + attributes.cGreenBits + attributes.cBlueBits + attributes.cAlphaBits;
						int score = std::abs(origAA - vm.AASamples) + std::abs(vm.BitsPerPixel - color);

						if (score < bestScore)
						{
							bestScore  = score;
							bestFormat = formats[i];
						}
					}
				}
			}
			else
			{
				LOG("Antialiasing is not supported");
				vm.AASamples = 0;
			}
		}

		if (bestFormat == 0)
		{
			PIXELFORMATDESCRIPTOR descriptor;
			ZeroMemory(&descriptor, sizeof(descriptor));
			descriptor.nSize        = sizeof(descriptor);
			descriptor.nVersion     = 1;
			descriptor.iLayerType   = PFD_MAIN_PLANE;
			descriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			descriptor.iPixelType   = PFD_TYPE_RGBA;
			descriptor.cColorBits   = static_cast<BYTE>(vm.BitsPerPixel);
			descriptor.cDepthBits   = static_cast<BYTE>(32);
			descriptor.cStencilBits = static_cast<BYTE>(24);
			descriptor.cAlphaBits   = vm.BitsPerPixel == 32 ? 8 : 0;

			bestFormat = ChoosePixelFormat(_DeviceContext, &descriptor);
			if (bestFormat == 0)
			{
				LOG("Failed to find a suitable pixel format for device context -- cannot create OpenGL context");
				return false;
			}
		}

		PIXELFORMATDESCRIPTOR actualFormat;
		actualFormat.nSize    = sizeof(actualFormat);
		actualFormat.nVersion = 1;
		DescribePixelFormat(_DeviceContext, bestFormat, sizeof(actualFormat), &actualFormat);

		if (!SetPixelFormat(_DeviceContext, bestFormat, &actualFormat))
		{
			LOG("Failed to set pixel format for device context -- cannot create OpenGL context");
			return false;
		}
	}

	HGLRC context = NULL;
	while (!context && (vm.GLMajor >= 3))
	{
        PFNWGLCREATECONTEXTATTRIBSARBPROC ccaarb = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
		if (ccaarb != NULL)
		{
			int attributes[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, vm.GLMajor,
				WGL_CONTEXT_MINOR_VERSION_ARB, vm.GLMinor,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				0
			};

			context = ccaarb(_DeviceContext, share != NULL ? share->_NativeHandle : NULL, attributes);
		}
		else
		{
			break;
		}

		if (!context)
		{
			if (vm.GLMajor > 0)
			{
				vm.GLMinor--;
			}
			else
			{
				vm.GLMajor--;
				vm.GLMinor = 9;
			}
		}
	}

	if (!context)
	{
		vm.GLMajor = 2;
		vm.GLMinor = 0;

		context = wglCreateContext(_DeviceContext);
		if (!context)
		{
			LOG("Failed to create an OpenGL context");
			return false;
		}

		if (share != NULL)
		{
			std::lock_guard<std::mutex> lock(_GLGuard);

			if (!wglShareLists(share->_NativeHandle, context))
			{
				LOG("Failed to share the OpenGL context");
				return false;
			}
		}
	}

	_NativeHandle = context;

	return true;
}

void GLContext::UnbindCurrent()
{
	wglMakeCurrent(NULL, NULL);
}

void GLContext::MakeCurrent()
{
	if (_DeviceContext != NULL && _NativeHandle != NULL)
		wglMakeCurrent(_DeviceContext, _NativeHandle);
}

void GLContext::Destroy()
{
	if (_NativeHandle != NULL)
		wglDeleteContext(_NativeHandle);
}

void GLContext::InitGlew()
{
	glewInit();
	wglewInit();
}

#endif
