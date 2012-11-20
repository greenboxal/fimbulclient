/*	This file is part of FimbulwinterClient.

	FimbulwinterClient is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	FimbulwinterClient is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FimbulwinterClient.  If not, see <http://www.gnu.org/licenses/>. */

#include <fclient/GuiManager.h>

#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>

#include <Awesomium/STLHelpers.h>

using namespace fclient;
using namespace YADE;
using namespace Awesomium;

int GetWebKeyFromSFMLKey(sf::Keyboard::Key key) 
{
#define mapKey(a, b) case sf::Keyboard::Key::##a: return Awesomium::KeyCodes::AK_##b;
	switch (key) 
	{
		mapKey(BackSpace, BACK)
		mapKey(Tab, TAB)
		mapKey(Return, RETURN)
		mapKey(Pause, PAUSE)
		mapKey(Escape, ESCAPE)
		mapKey(Space, SPACE)
		mapKey(Comma, OEM_COMMA)
		mapKey(Dash, OEM_MINUS)
		mapKey(Period, OEM_PERIOD)
		mapKey(Slash, OEM_2)
		mapKey(Num0, 0)
		mapKey(Num1, 1)
		mapKey(Num2, 2)
		mapKey(Num3, 3)
		mapKey(Num4, 4)
		mapKey(Num5, 5)
		mapKey(Num6, 6)
		mapKey(Num7, 7)
		mapKey(Num8, 8)
		mapKey(Num9, 9)
		mapKey(SemiColon, OEM_1)
		mapKey(Equal, OEM_PLUS)
		mapKey(LBracket, OEM_4)
		mapKey(BackSlash, OEM_5)
		mapKey(RBracket, OEM_6)
		mapKey(Quote, OEM_3)
		mapKey(A, A)
		mapKey(B, B)
		mapKey(C, C)
		mapKey(D, D)
		mapKey(E, E)
		mapKey(F, F)
		mapKey(G, G)
		mapKey(H, H)
		mapKey(I, I)
		mapKey(J, J)
		mapKey(K, K)
		mapKey(L, L)
		mapKey(M, M)
		mapKey(N, N)
		mapKey(O, O)
		mapKey(P, P)
		mapKey(Q, Q)
		mapKey(R, R)
		mapKey(S, S)
		mapKey(T, T)
		mapKey(U, U)
		mapKey(V, V)
		mapKey(W, W)
		mapKey(X, X)
		mapKey(Y, Y)
		mapKey(Z, Z)
		mapKey(Delete, DELETE)
		mapKey(Numpad0, NUMPAD0)
		mapKey(Numpad1, NUMPAD1)
		mapKey(Numpad2, NUMPAD2)
		mapKey(Numpad3, NUMPAD3)
		mapKey(Numpad4, NUMPAD4)
		mapKey(Numpad5, NUMPAD5)
		mapKey(Numpad6, NUMPAD6)
		mapKey(Numpad7, NUMPAD7)
		mapKey(Numpad8, NUMPAD8)
		mapKey(Numpad9, NUMPAD9)
		mapKey(Divide, DIVIDE)
		mapKey(Multiply, MULTIPLY)
		mapKey(Subtract, SUBTRACT)
		mapKey(Add, ADD)
		mapKey(Up, UP)
		mapKey(Down, DOWN)
		mapKey(Right, RIGHT)
		mapKey(Left, LEFT)
		mapKey(Insert, INSERT)
		mapKey(Home, HOME)
		mapKey(End, END)
		mapKey(PageUp, PRIOR)
		mapKey(PageDown, NEXT)
		mapKey(F1, F1)
		mapKey(F2, F2)
		mapKey(F3, F3)
		mapKey(F4, F4)
		mapKey(F5, F5)
		mapKey(F6, F6)
		mapKey(F7, F7)
		mapKey(F8, F8)
		mapKey(F9, F9)
		mapKey(F10, F10)
		mapKey(F11, F11)
		mapKey(F12, F12)
		mapKey(F13, F13)
		mapKey(F14, F14)
		mapKey(F15, F15)
		mapKey(RShift, RSHIFT)
		mapKey(LShift, LSHIFT)
		mapKey(RControl, RCONTROL)
		mapKey(LControl, LCONTROL)
		mapKey(RAlt, RMENU)
		mapKey(LAlt, LMENU)
		mapKey(LSystem, LWIN)
		mapKey(RSystem, RWIN)
	default:
		return Awesomium::KeyCodes::AK_UNKNOWN;
	}
#undef mapKey
}

class OpenGLSurface : public Surface
{
public:
	OpenGLSurface(GuiManager *owner, int width, int height)
	{
		_Owner = owner;
		_Texture.create(width, height);
		_Buffer = new unsigned char[width * height * 4];
	}

	virtual ~OpenGLSurface()
	{
		delete[] _Buffer;
	}

	unsigned char GetPixelAlpha(int x, int y)
	{
		return _Buffer[(y * _Texture.getSize().x + x) * 4 + 3];
	}

	virtual void Paint(unsigned char *src_buffer, int src_row_span, const Awesomium::Rect &src_rect, const Awesomium::Rect &dest_rect)
	{
		int w = _Texture.getSize().x;
		int stride = src_row_span / 4;

		for (int y = 0; y < dest_rect.height; y++)
		{
			size_t soff = ((src_rect.y + y) * stride + src_rect.x) * 4;
			size_t doff = ((dest_rect.y + y) * w + dest_rect.x) * 4;

			for (int x = 0; x < dest_rect.width; x++, soff += 4, doff += 4)
			{
				_Buffer[doff + 0] = src_buffer[soff + 2];
				_Buffer[doff + 1] = src_buffer[soff + 1];
				_Buffer[doff + 2] = src_buffer[soff + 0];
				_Buffer[doff + 3] = src_buffer[soff + 3];
			}
		}

		Invalidate(dest_rect.x, dest_rect.y, dest_rect.width, dest_rect.height);
	}

	virtual void Scroll(int dx, int dy, const Awesomium::Rect &clip_rect)
	{
		int w = _Texture.getSize().x;

		for (int y = 0; y < clip_rect.height; y++)
			memmove(&_Buffer[((dy + y) * w + dx) * 4], &_Buffer[((clip_rect.y + y) * w + clip_rect.x) * 4], clip_rect.width * 4);

		Invalidate(dx, dy, clip_rect.width, clip_rect.height);
	}

	void Render()
	{
		_Owner->Window().draw(sf::Sprite(_Texture));
	}

private:
	void Invalidate(int x, int y, int w, int h)
	{
		// FIXME: Only update the changed area
		_Texture.update(_Buffer, _Texture.getSize().x, _Texture.getSize().y, 0, 0);
	}

	GuiManager *_Owner;
	sf::Texture _Texture;
	unsigned char *_Buffer;
};

GuiManager::GuiManager(sf::RenderWindow &window)
	: _Window(window)
{
	_StartedInGui[0] = false;
	_StartedInGui[1] = false;
	_StartedInGui[2] = false;
}

void GuiManager::SetDesktop(const std::string &desktop)
{
	_View->LoadURL(WebURL(ToWebString("asset://fclient/ui/" + desktop + ".html")));
}

void GuiManager::Load()
{
	WebConfig config;

#ifdef _DEBUG
	config.log_level = kLogLevel_Verbose;
#else
	config.log_level = kLogLevel_Normal;
#endif

	LOG("Initializing WebCore");
	WebCore::Initialize(config);
	WebCore::instance()->set_surface_factory(this);

	_DataSource = new YadeDataSource();

	_View = WebCore::instance()->CreateWebView(_Window.getSize().x, _Window.getSize().y);
	_View->SetTransparent(true);
	_View->session()->AddDataSource(WSLit("fclient"), _DataSource);
}

bool GuiManager::DispatchEvent(const sf::Event &e)
{
	if (e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased || e.type == sf::Event::TextEntered)
	{
		WebKeyboardEvent wke;

		switch (_View->focused_element_type())
		{
		case kFocusedElementType_TextInput:
		case kFocusedElementType_EditableContent:
		case kFocusedElementType_Input:
			break;
		default:
			return false;
		}

		char buffer[20];
		char *buf = buffer;
		wke.virtual_key_code = GetWebKeyFromSFMLKey(e.key.code);
		GetKeyIdentifierFromVirtualKeyCode(wke.virtual_key_code, &buf);
		strcpy(wke.key_identifier, buf);

		wke.modifiers = 0;
		if (e.key.alt)
			wke.modifiers |= Awesomium::WebKeyboardEvent::kModAltKey;
		if (e.key.control)
			wke.modifiers |= Awesomium::WebKeyboardEvent::kModControlKey;
		if (e.key.system)
			wke.modifiers |= Awesomium::WebKeyboardEvent::kModMetaKey;
		if (e.key.shift)
			wke.modifiers |= Awesomium::WebKeyboardEvent::kModShiftKey;

		wke.native_key_code = wke.virtual_key_code;

		if (e.type == sf::Event::KeyPressed)
		{
			wke.type = WebKeyboardEvent::kTypeKeyDown;
		}
		else if (e.type == sf::Event::KeyReleased)
		{
			wke.type = WebKeyboardEvent::kTypeKeyUp;
		}
		else
		{
			wke.type = WebKeyboardEvent::kTypeChar;
			wke.text[0] = (wchar16)e.text.unicode;
			wke.unmodified_text[0] = (wchar16)e.text.unicode;
		}

		_View->InjectKeyboardEvent(wke);
	}
	else if (e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased || e.type == sf::Event::MouseWheelMoved)
	{
		int x, y;
		MouseButton btn;

		if (e.type == sf::Event::MouseWheelMoved)
		{
			x = e.mouseWheel.x;
			y = e.mouseWheel.y;
		}
		else
		{
			x = e.mouseButton.x;
			y = e.mouseButton.y;
			
			switch (e.mouseButton.button)
			{
			case sf::Mouse::Left: btn = kMouseButton_Left; break;
			case sf::Mouse::Middle: btn = kMouseButton_Middle; break;
			case sf::Mouse::Right: btn = kMouseButton_Right; break;
			}
		}

		if (((OpenGLSurface *)_View->surface())->GetPixelAlpha(x, y) < 10)
		{
			if (e.type == sf::Event::MouseButtonPressed)
				_StartedInGui[btn] = false;

			if (e.type == sf::Event::MouseButtonReleased && _StartedInGui[btn])
				_View->InjectMouseUp(btn);

			_View->Unfocus();
			return false;
		}
		else if (e.type == sf::Event::MouseButtonPressed)
		{
			_StartedInGui[btn] = true;
			_View->Focus();
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			if (!_StartedInGui[btn])
				return false;
		}

		if (e.type == sf::Event::MouseWheelMoved)
			_View->InjectMouseWheel(e.mouseWheel.delta * 100, 0);
		else if (e.type == sf::Event::MouseButtonPressed)
			_View->InjectMouseDown(btn);
		else
			_View->InjectMouseUp(btn);
	}
	else if (e.type == sf::Event::MouseMoved)
	{
		_View->InjectMouseMove(e.mouseMove.x, e.mouseMove.y);
	}

	return true;
}

void GuiManager::Update()
{
	WebCore::instance()->Update();
}

void GuiManager::Render()
{
	OpenGLSurface *surface = (OpenGLSurface *)_View->surface();

	if (surface != nullptr)
		surface->Render();
}

void GuiManager::Unload()
{
	delete _DataSource;
	WebCore::Shutdown();
}

Awesomium::Surface *GuiManager::CreateSurface(Awesomium::WebView *view, int width, int height)
{
	return new OpenGLSurface(this, width, height);
}

void GuiManager::DestroySurface(Awesomium::Surface *surface)
{
	delete (OpenGLSurface *)surface;
}
