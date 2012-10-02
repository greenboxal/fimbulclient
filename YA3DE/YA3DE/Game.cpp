/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#include <YA3DE/Game.h>
#include <YA3DE/FileSystem/FileManager.h>

#include <exception>

using namespace YA3DE;
using namespace YA3DE::FileSystem;

Game *Game::_instance = NULL;

Game::Game()
{
	if (_instance != NULL)
		throw std::exception("YA3DE::Game must have only one instance.");

	_instance = this;
}

Game::~Game()
{

}

void Game::ReadConfig()
{
	rapidxml::xml_node<> *config = _Configuration->GetRoot().first_node("Config");

	rapidxml::xml_node<> *window = config->first_node("Window");
	if (window)
	{
		int style = sf::Style::Close | sf::Style::Titlebar;
		std::string title = window->first_attribute("Title")->value();

		if (stricmp(window->first_attribute("Fullscreen")->value(), "true") == 0)
			style |= sf::Style::Fullscreen;

		_Window = new sf::RenderWindow(
			sf::VideoMode(
				atoi(window->first_attribute("Width")->value()), 
				atoi(window->first_attribute("Height")->value()), 
				atoi(window->first_attribute("BitsPerPixel")->value())),
			title,
			style);

		_Window->setVerticalSyncEnabled(true);

		glewInit();
	}

	rapidxml::xml_node<> *fileSystem = config->first_node("FileSystem");
	if (fileSystem)
	{
		rapidxml::xml_node<> *sources = fileSystem->first_node("Sources");
		if (sources)
		{
			for (rapidxml::xml_node<> *source = sources->first_node(NULL); source; source = source->next_sibling())
			{
				FileManager::Instance()->LoadFileSystem(source->first_attribute("Type")->value(), source->first_attribute("Path")->value());
			}
		}
	}
}

void Game::Run()
{
	double total = 0, frames = 0;

	OnInitialize();
	ReadConfig();
	OnLoad();

	_Running = true;
	
	sf::Clock clock;
	while (_Window->isOpen() && _Running)
	{
		sf::Event ev;
		double elapsed = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		while(_Window->pollEvent(ev) && _Running)
		{
			if (ev.type == sf::Event::GainedFocus)
				_IsActive = true;
			else if (ev.type == sf::Event::LostFocus)
				_IsActive = false;

			OnEvent(ev, elapsed);
		}

		if (!_Running)
			break;

		OnUpdate(elapsed);
		OnRender(elapsed);

		_Window->display();

		total += elapsed;
		frames++;

		if (total > 1000.0F)
		{
			_FrameRate = frames;
			frames = 0;
			total -= 1000.0F;
		}
	}

	OnUnload();

	_Window->close();
	delete _Window;
}
