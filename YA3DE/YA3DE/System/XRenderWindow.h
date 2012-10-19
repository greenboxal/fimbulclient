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

#ifndef _XRENDERWINDOW_H_
#define _XRENDERWINDOW_H_

#include <mutex>
#include <queue>
#include <vector>
#include <string>

#include <X11/Xlib.h>

#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>
#include <YA3DE/System/Event.h>
#include <YA3DE/System/VideoMode.h>

namespace YA3DE
{
	namespace System
	{
		namespace WindowStyle
		{
			enum _WindowStyle
			{
				None = 0,
				Borderless = 0x1,
				FullScreen = 0x2,
				Resizable = 0x4,
				CloseButton = 0x8,
				Titlebar = 0x10,
			};
		}

		class GLContext;
		class RenderWindow
		{
		public:
			RenderWindow();
			~RenderWindow();

			bool Create(const VideoMode &mode, int style = WindowStyle::None);
			GLContext *AllocContext(bool shared);

			void SetTitle(const std::string &title);
			void SetMouseCursor(bool show);
			void SetVSync(bool sync);

			void EndScene();
			void Close();

			bool PollEvent(Event &e);
			
			DEFPROP_RO(public, VideoMode, Mode);
			DEFPROP_RO(public, GLContext *, MainContext);
			DEFPROP_RO(public, std::vector<GLContext *>, ContextList);
			
			DEFPROP_RO(public, Display *, DeviceContext);
			DEFPROP_RO(public, Window, NativeHandle);
			DEFPROP_RO(public, bool, IsActive);

			DEFPROP_RO(public, glm::uvec2, Position);
			DEFPROP_RO(public, glm::uvec2, Size);

			DEFPROP_RW(public, bool, KeyRepeat);

		private:
			bool _VSync;
			std::queue<Event> _EventQueue;
		};
	}
}

#endif
