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

#ifndef _WIN32MOUSE_H_
#define _WIN32MOUSE_H_

#include <YA3DE/Math.h>

namespace YA3DE
{
	namespace System
	{
		enum class ButtonState
		{
			Released,
			Pressed
		};

		struct MouseState
		{
			ButtonState LeftButton;
			ButtonState MiddleButton;
			ButtonState RightButton;
			ButtonState XButton1;
			ButtonState XButton2;
			int ScrollWheelValue;
			int X;
			int Y;
		};

		class Mouse
		{
		public:
			static void GetState(MouseState &state);
			static void SetPosition(const glm::uvec2 &pos);

			static void UpdateMouseWheel(int delta);

		private:
			static int _MouseWheel;
		};
	}
}

#endif
