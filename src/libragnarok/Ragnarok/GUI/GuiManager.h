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

#ifndef FCLIENT_GUIMANAGER_H
#define FCLIENT_GUIMANAGER_H

#include <YA3DE/Helpers.h>
#include <SFML/Graphics.hpp>
#include <Ragnarok/GUI/Ya3deDataSource.h>
#include <Ragnarok/GUI/JSDispatcher.h>

namespace Ragnarok
{
	using namespace Awesomium;

	class GuiManager : private Awesomium::SurfaceFactory
	{
	public:
		GuiManager(sf::RenderWindow &window);

		void Load();
		bool DispatchEvent(const sf::Event &e);
		void Update();
		void Render();
		void Unload();

		bool TestPixel(int x, int y);
		void SetDesktop(const std::string &desktop);

		virtual Surface *CreateSurface(WebView *view, int width, int height);
		virtual void DestroySurface(Surface *surface);

		sf::RenderWindow &Window()
		{
			return _Window;
		}

		DEFPROP_RO_P(public, JSDispatcher, Dispatcher);
		DEFPROP_RO_P(public, WebView, View);

	private:
		bool _StartedInGui[3];
		sf::RenderWindow &_Window;
		YadeDataSource *_DataSource;
		JSObject _ApplicationObject;
	};
}

#endif
