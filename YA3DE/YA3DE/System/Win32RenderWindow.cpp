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

#include <stdafx.h>

#ifdef _WIN32

#include <YA3DE/OpenGL.h>
#include <YA3DE/Logger.h>
#include <YA3DE/System/Mouse.h>
#include <YA3DE/System/GLContext.h>
#include <YA3DE/System/RenderWindow.h>

using namespace YA3DE::System;

static unsigned int _WindowClassCounter = 0;

RenderWindow::RenderWindow()
{
	_NativeHandle = NULL;
	_Cursor = NULL;
	_MouseCursorIn = false;
	_KeyRepeat = false;

	_MainContext = NULL;
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
	_ClassName = "YA3DEWIN_" + _WindowClassCounter++;
	_Mode = mode;

	WNDCLASSA windowClass;
    windowClass.style         = 0;
    windowClass.lpfnWndProc   = &RenderWindow::WndProc;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.hIcon         = NULL;
    windowClass.hCursor       = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = _ClassName.c_str();
	RegisterClassA(&windowClass);

	HDC screenDC = GetDC(NULL);
	int width = _Mode.Width;
	int height = _Mode.Height;
	int left = (GetDeviceCaps(screenDC, HORZRES) - width)  / 2;
	int top = (GetDeviceCaps(screenDC, VERTRES) - height) / 2;
	ReleaseDC(NULL, screenDC);

	DWORD win32style = WS_VISIBLE;

	if (style == WindowStyle::None)
	{
		win32style |= WS_POPUP;
	}
	else
	{
		if (style & WindowStyle::Titlebar)
			win32style |= WS_CAPTION | WS_MINIMIZEBOX;

		if (style & WindowStyle::Resizable)
		{
			throw std::exception("");
			win32style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
		}

		if (style & WindowStyle::CloseButton)
			win32style |= WS_SYSMENU;
	}

	if ((style & WindowStyle::FullScreen) == 0)
	{
		RECT rect = { 0, 0, width, height };

		AdjustWindowRect(&rect, win32style, false);

		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	_NativeHandle = CreateWindowA(_ClassName.c_str(), "YA3DE - Yet Another 3D Engine", win32style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), this);

	if (_NativeHandle == NULL)
		return false;

	if (style & WindowStyle::FullScreen)
	{
		DEVMODE devMode;
		devMode.dmSize       = sizeof(devMode);
		devMode.dmPelsWidth  = width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = _Mode.BitsPerPixel;
		devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			return false;

		SetWindowLong(_NativeHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowLong(_NativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);

		SetWindowPos(_NativeHandle, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
		ShowWindow(_NativeHandle, SW_SHOW);
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
	SetWindowTextA(_NativeHandle, title.c_str());
}

void RenderWindow::SetMouseCursor(bool show)
{
	if (show)
		_Cursor = LoadCursor(NULL, IDC_ARROW);
	else
		_Cursor = NULL;

	SetCursor(_Cursor);
}

void RenderWindow::SetVSync(bool sync)
{
	wglSwapIntervalEXT(sync ? 1 : 0);
}

bool RenderWindow::PollEvent(Event &e)
{
	if (_EventQueue.size() == 0)
	{
		MSG message;
		while (PeekMessage(&message, _NativeHandle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
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

glm::uvec2 RenderWindow::GetPosition()
{
	RECT r;
	GetWindowRect(_NativeHandle, &r);

	return glm::uvec2(r.left, r.top);
}

glm::uvec2 RenderWindow::GetSize()
{
	RECT r;
	GetWindowRect(_NativeHandle, &r);

	return glm::uvec2(r.right - r.left, r.bottom - r.top);
}

void RenderWindow::EndScene()
{
	SwapBuffers(GetDC(_NativeHandle));
}

void RenderWindow::Close()
{
	if (_NativeHandle != NULL)
	{
		ChangeDisplaySettings(NULL, 0);

		DestroyWindow(_NativeHandle);
		_NativeHandle = NULL;

		UnregisterClassA(_ClassName.c_str(), NULL);

		SetMouseCursor(true);
	}
}

void RenderWindow::_WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_NativeHandle == NULL)
		return;

	switch (message)
	{
	case WM_CREATE:
		{
			_IsActive = true;
		}
		break;
	case WM_SETCURSOR:
		{
		if (LOWORD(lParam) == HTCLIENT)
			SetCursor(_Cursor);
		}
		break;
	case WM_CLOSE:
		{
			Event e;
			e.Type = EventType::Close;
			_EventQueue.push(e);
		}
		break;
	case WM_SETFOCUS:
		{
			// PUSHEVENT
			_IsActive = true;
		}
		break;
	case WM_KILLFOCUS:
		{
			// PUSHEVENT
			_IsActive = false;
		}
		break;
	case WM_CHAR:
		{
			if (_KeyRepeat || ((lParam & (1 << 30)) == 0))
			{
				// PUSHEVENT
			}
		}
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			if (_KeyRepeat || ((HIWORD(lParam) & KF_REPEAT) == 0))
			{
				// PUSHEVENT
			}
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			if (_KeyRepeat || ((HIWORD(lParam) & KF_REPEAT) == 0))
			{
				// PUSHEVENT
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT position;
            position.x = static_cast<short>(LOWORD(lParam));
            position.y = static_cast<short>(HIWORD(lParam));
			ScreenToClient(_NativeHandle, &position);

			Mouse::UpdateMouseWheel(HIWORD(wParam) / 120);

			// PUSHEVENT
		}
		break;
	case WM_LBUTTONDOWN:
		{
			// PUSHEVENT
		}
		break;
	case WM_LBUTTONUP:
		{
			// PUSHEVENT
		}
		break;
	case WM_RBUTTONDOWN:
		{
			// PUSHEVENT
		}
		break;
	case WM_RBUTTONUP:
		{
			// PUSHEVENT
		}
		break;
	case WM_MBUTTONDOWN:
		{
			// PUSHEVENT
		}
		break;
	case WM_MBUTTONUP:
		{
			// PUSHEVENT
		}
		break;
	case WM_XBUTTONDOWN:
		{
			// PUSHEVENT
		}
		break;
	case WM_XBUTTONUP:
		{
			// PUSHEVENT
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (!_MouseCursorIn)
			{
                TRACKMOUSEEVENT mouseEvent;
                mouseEvent.cbSize    = sizeof(TRACKMOUSEEVENT);
                mouseEvent.hwndTrack = _NativeHandle;
                mouseEvent.dwFlags   = TME_LEAVE;
                TrackMouseEvent(&mouseEvent);

				_MouseCursorIn = true;

				// PUSHEVENT
			}

			// PUSHEVENT
		}
		break;
	case WM_MOUSELEAVE:
		{
			_MouseCursorIn = false;

			// PUSHEVENT
		}
		break;
	}
}

LRESULT CALLBACK RenderWindow::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)((CREATESTRUCTA *)lParam)->lpCreateParams);

	RenderWindow *window = (RenderWindow *)GetWindowLongPtr(handle, GWLP_USERDATA);

	if (window)
		window->_WndProc(message, wParam, lParam);
	
	if (message == WM_CLOSE)
		return 0;

	return DefWindowProcA(handle, message, wParam, lParam);
}

#endif
