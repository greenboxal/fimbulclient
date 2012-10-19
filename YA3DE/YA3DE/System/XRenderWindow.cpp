/*
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

#ifdef _LINUX

#include <YA3DE/OpenGL.h>
#include <YA3DE/Logger.h>
#include <YA3DE/System/Mouse.h>
#include <YA3DE/System/GLContext.h>
#include <YA3DE/System/RenderWindow.h>

#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>

using namespace YA3DE::System;

RenderWindow::RenderWindow()
{
	_DeviceContext = NULL;
	_NativeHandle = NULL;
	_MainContext = NULL;
	_MouseCursorIn = false;
	_IsActive = false;
	_KeyRepeat = false;
	_VSync = false;
}

RenderWindow::~RenderWindow()
{
	if (_NativeHandle)
		Close();

	for (unsigned int i = 0; i < _ContextList.size(); i++)
	{
		_ContextList[i]->Destroy();
		delete _ContextList[i];
	}
}

bool RenderWindow::Create(const VideoMode &mode, int style)
{
	_Mode = mode;
	_DeviceContext = XOpenDisplay(NULL);

	int screen;
	XVisualInfo *vi;
	Colormap cmap;
	int i, dpyWidth, dpyHeight;
	int glxMajor, glxMinor, vmMajor, vmMinor;
	XF86VidModeModeInfo **modes;
	int modeNum, bestMode;
	Atom wmDelete;
	Window winDummy;
	unsigned int borderDummy;
	XSetWindowAttributes winAttr;

	static int attrListDbl[] =
	{
	    GLX_RGBA, GLX_DOUBLEBUFFER,
	    GLX_RED_SIZE, 4,
	    GLX_GREEN_SIZE, 4,
	    GLX_BLUE_SIZE, 4,
	    GLX_DEPTH_SIZE, 32,
	    None
	};

	bestMode = 0;
	screen = DefaultScreen(_DeviceContext);
	XF86VidModeQueryVersion(_DeviceContext, &vmMajor, &vmMinor);
	XF86VidModeGetAllModeLines(_DeviceContext, screen, &modeNum, &modes);

	for (i = 0; i < modeNum; i++)
	{
		if ((modes[i]->hdisplay == mode.Width) && (modes[i]->vdisplay == mode.Height))
			bestMode = i;
	}

	vi = glXChooseVisual(_DeviceContext, screen, attrListDbl);
	if (vi == NULL)
	{
		LOG("Doublebuffered rendering not avaiable, exiting");
		return false;
	}

	glXQueryVersion(_DeviceContext, &glxMajor, &glxMinor);
	LOG("GLX-Version %d.%d", glxMajor, glxMinor);

	cmap = XCreateColormap(_DeviceContext, RootWindow(_DeviceContext, vi->screen), vi->visual, AllocNone);
	winAttr.colormap = cmap;
	winAttr.border_pixel = 0;

	if (mode.FullScreen)
	{
		XF86VidModeSwitchToMode(_DeviceContext, screen, modes[bestMode]);
		XF86VidModeSetViewPort(_DeviceContext, screen, 0, 0);
		dpyWidth = modes[bestMode]->hdisplay;
		dpyHeight = modes[bestMode]->vdisplay;
		LOG("Resolution %dx%d\n", dpyWidth, dpyHeight);
		XFree(modes);

		winAttr.override_redirect = True;
		winAttr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

		_NativeHandle = XCreateWindow(_DeviceContext, RootWindow(_DeviceContext, vi->screen),
			0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
			&winAttr);

		XWarpPointer(_DeviceContext, None, _NativeHandle, 0, 0, 0, 0, 0, 0);
		XMapRaised(_DeviceContext, _NativeHandle);
		XGrabKeyboard(_DeviceContext, _NativeHandle, True, GrabModeAsync, GrabModeAsync, CurrentTime);
		XGrabPointer(_DeviceContext, _NativeHandle, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, _NativeHandle, None, CurrentTime);
	}
	else
	{
		winAttr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

		_NativeHandle = XCreateWindow(_DeviceContext, RootWindow(_DeviceContext, vi->screen),
			0, 0, mode.Width, mode.Height, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask, &winAttr);

		wmDelete = XInternAtom(_DeviceContext, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(_DeviceContext, _NativeHandle, &wmDelete, 1);
		XSetStandardProperties(_DeviceContext, _NativeHandle, "YA3DE - Yet Another 3D Engine", "YA3DE - Yet Another 3D Engine", None, NULL, 0, NULL);
		XMapRaised(_DeviceContext, _NativeHandle);
	}

	_MainContext = AllocContext(false);

	if (_MainContext == NULL)
		return false;

	_MainContext->MakeCurrent();
	_MainContext->InitGlew();

	return true;
}

GLContext *RenderWindow::AllocContext(bool shared)
{
	GLContext *ctx = new GLContext(this);

	if (!ctx->Create(shared ? _MainContext : NULL))
	{
		delete ctx;
		return NULL;
	}

	_ContextList.push_back(ctx);

	return ctx;
}

void RenderWindow::SetTitle(const std::string &title)
{
	XSizeHints sizehints;
	sizehints.flags = 0;
	XSetStandardProperties(_DeviceContext, _NativeHandle, title.c_str(), title.c_str(), None, (char **)NULL, 0, &sizehints);
}

void RenderWindow::SetMouseCursor(bool show)
{
	if (show)
	{
		XDefineCursor(_DeviceContext, _NativeHandle, None);
	}
	else
	{
		static char data[1] = {0};
		Cursor cursor;
		Pixmap blank;
		XColor dummy;

		blank = XCreateBitmapFromData(_DeviceContext, _NativeHandle, data, 1, 1);
		cursor = XCreatePixmapCursor(_DeviceContext, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(_DeviceContext, blank);

		XDefineCursor(_DeviceContext, _NativeHandle, cursor);
	}
}

void RenderWindow::SetVSync(bool sync)
{
	_VSync = sync;
}

bool RenderWindow::PollEvent(Event &e)
{
	if (_EventQueue.size() == 0)
	{
		XEvent xe;

		while (XPending(_DeviceContext) > 0)
		{
			XNextEvent(_DeviceContext, &xe);
		}
	}
	
	if (_EventQueue.size() > 0)
	{
		e = _EventQueue.front();
		_EventQueue.pop();
	}
	else
	{
		return false;
	}

	return true;
}

void RenderWindow::EndScene()
{
	glXSwapBuffers(_DeviceContext, _NativeHandle);

	if (_VSync)
	{
		int retraceCount;
		glXGetVideoSyncSGI(&retraceCount);
		glXWaitVideoSyncSGI(2, (retraceCount+1)%2, &retraceCount);
	}
}

void RenderWindow::Close()
{
	if (_NativeHandle != NULL)
	{
		XDestroyWindow(_DeviceContext, _NativeHandle);
		XCloseDisplay(_DeviceContext);

		_NativeHandle = NULL;
		_DeviceContext = NULL;
	}
}

#endif
