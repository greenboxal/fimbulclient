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
#include <YA3DE/System/Keyboard.h>

using namespace YA3DE::System;

KeyState Keyboard::GetKeyState(Key key)
{
	int vkey = 0;

	switch (key)
	{
	default:              vkey = 0;             break;
	case Key::A:          vkey = 'A';           break;
	case Key::B:          vkey = 'B';           break;
	case Key::C:          vkey = 'C';           break;
	case Key::D:          vkey = 'D';           break;
	case Key::E:          vkey = 'E';           break;
	case Key::F:          vkey = 'F';           break;
	case Key::G:          vkey = 'G';           break;
	case Key::H:          vkey = 'H';           break;
	case Key::I:          vkey = 'I';           break;
	case Key::J:          vkey = 'J';           break;
	case Key::K:          vkey = 'K';           break;
	case Key::L:          vkey = 'L';           break;
	case Key::M:          vkey = 'M';           break;
	case Key::N:          vkey = 'N';           break;
	case Key::O:          vkey = 'O';           break;
	case Key::P:          vkey = 'P';           break;
	case Key::Q:          vkey = 'Q';           break;
	case Key::R:          vkey = 'R';           break;
	case Key::S:          vkey = 'S';           break;
	case Key::T:          vkey = 'T';           break;
	case Key::U:          vkey = 'U';           break;
	case Key::V:          vkey = 'V';           break;
	case Key::W:          vkey = 'W';           break;
	case Key::X:          vkey = 'X';           break;
	case Key::Y:          vkey = 'Y';           break;
	case Key::Z:          vkey = 'Z';           break;
	case Key::Num0:       vkey = '0';           break;
	case Key::Num1:       vkey = '1';           break;
	case Key::Num2:       vkey = '2';           break;
	case Key::Num3:       vkey = '3';           break;
	case Key::Num4:       vkey = '4';           break;
	case Key::Num5:       vkey = '5';           break;
	case Key::Num6:       vkey = '6';           break;
	case Key::Num7:       vkey = '7';           break;
	case Key::Num8:       vkey = '8';           break;
	case Key::Num9:       vkey = '9';           break;
	case Key::Escape:     vkey = VK_ESCAPE;     break;
	case Key::LControl:   vkey = VK_LCONTROL;   break;
	case Key::LShift:     vkey = VK_LSHIFT;     break;
	case Key::LAlt:       vkey = VK_LMENU;      break;
	case Key::LSystem:    vkey = VK_LWIN;       break;
	case Key::RControl:   vkey = VK_RCONTROL;   break;
	case Key::RShift:     vkey = VK_RSHIFT;     break;
	case Key::RAlt:       vkey = VK_RMENU;      break;
	case Key::RSystem:    vkey = VK_RWIN;       break;
	case Key::Menu:       vkey = VK_APPS;       break;
	case Key::LBracket:   vkey = VK_OEM_4;      break;
	case Key::RBracket:   vkey = VK_OEM_6;      break;
	case Key::SemiColon:  vkey = VK_OEM_1;      break;
	case Key::Comma:      vkey = VK_OEM_COMMA;  break;
	case Key::Period:     vkey = VK_OEM_PERIOD; break;
	case Key::Quote:      vkey = VK_OEM_7;      break;
	case Key::Slash:      vkey = VK_OEM_2;      break;
	case Key::BackSlash:  vkey = VK_OEM_5;      break;
	case Key::Tilde:      vkey = VK_OEM_3;      break;
	case Key::Equal:      vkey = VK_OEM_PLUS;   break;
	case Key::Dash:       vkey = VK_OEM_MINUS;  break;
	case Key::Space:      vkey = VK_SPACE;      break;
	case Key::Return:     vkey = VK_RETURN;     break;
	case Key::BackSpace:  vkey = VK_BACK;       break;
	case Key::Tab:        vkey = VK_TAB;        break;
	case Key::PageUp:     vkey = VK_PRIOR;      break;
	case Key::PageDown:   vkey = VK_NEXT;       break;
	case Key::End:        vkey = VK_END;        break;
	case Key::Home:       vkey = VK_HOME;       break;
	case Key::Insert:     vkey = VK_INSERT;     break;
	case Key::Delete:     vkey = VK_DELETE;     break;
	case Key::Add:        vkey = VK_ADD;        break;
	case Key::Subtract:   vkey = VK_SUBTRACT;   break;
	case Key::Multiply:   vkey = VK_MULTIPLY;   break;
	case Key::Divide:     vkey = VK_DIVIDE;     break;
	case Key::Left:       vkey = VK_LEFT;       break;
	case Key::Right:      vkey = VK_RIGHT;      break;
	case Key::Up:         vkey = VK_UP;         break;
	case Key::Down:       vkey = VK_DOWN;       break;
	case Key::Numpad0:    vkey = VK_NUMPAD0;    break;
	case Key::Numpad1:    vkey = VK_NUMPAD1;    break;
	case Key::Numpad2:    vkey = VK_NUMPAD2;    break;
	case Key::Numpad3:    vkey = VK_NUMPAD3;    break;
	case Key::Numpad4:    vkey = VK_NUMPAD4;    break;
	case Key::Numpad5:    vkey = VK_NUMPAD5;    break;
	case Key::Numpad6:    vkey = VK_NUMPAD6;    break;
	case Key::Numpad7:    vkey = VK_NUMPAD7;    break;
	case Key::Numpad8:    vkey = VK_NUMPAD8;    break;
	case Key::Numpad9:    vkey = VK_NUMPAD9;    break;
	case Key::F1:         vkey = VK_F1;         break;
	case Key::F2:         vkey = VK_F2;         break;
	case Key::F3:         vkey = VK_F3;         break;
	case Key::F4:         vkey = VK_F4;         break;
	case Key::F5:         vkey = VK_F5;         break;
	case Key::F6:         vkey = VK_F6;         break;
	case Key::F7:         vkey = VK_F7;         break;
	case Key::F8:         vkey = VK_F8;         break;
	case Key::F9:         vkey = VK_F9;         break;
	case Key::F10:        vkey = VK_F10;        break;
	case Key::F11:        vkey = VK_F11;        break;
	case Key::F12:        vkey = VK_F12;        break;
	case Key::F13:        vkey = VK_F13;        break;
	case Key::F14:        vkey = VK_F14;        break;
	case Key::F15:        vkey = VK_F16;        break;
	case Key::Pause:      vkey = VK_PAUSE;      break;
	}

	return (GetAsyncKeyState(vkey) & 0x8000) != 0 ? KeyState::Pressed : KeyState::Released;
}

#endif
