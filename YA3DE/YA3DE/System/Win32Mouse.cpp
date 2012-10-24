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

#include <Windows.h>

#include <YA3DE/Game.h>
#include <YA3DE/System/Mouse.h>

using namespace YA3DE::System;

int Mouse::_MouseWheel = 0;

void Mouse::GetState(MouseState &state)
{
	bool swap = GetSystemMetrics(SM_SWAPBUTTON) == TRUE;

	if (swap)
	{
		state.LeftButton = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;
		state.RightButton = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;
	}
	else
	{
		state.LeftButton = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;
		state.RightButton = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;
	}
	
	state.MiddleButton = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;
	state.XButton1 = (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;
	state.XButton2 = (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) != 0 ? ButtonState::Pressed : ButtonState::Released;

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(Game::Instance()->Window.NativeHandle, &p);

	state.X = p.x;
	state.Y = p.y;

	state.ScrollWheelValue = _MouseWheel;
}

void Mouse::SetPosition(const glm::uvec2 &pos)
{
	glm::uvec2 wpos = Game::Instance()->Window.Position;
	SetCursorPos(pos.x + wpos.x, pos.y + wpos.y);
}

void Mouse::UpdateMouseWheel(int delta)
{
	_MouseWheel += delta;
}

#endif
