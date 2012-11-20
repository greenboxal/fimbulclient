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

#include <YA3DE/Application.h>
#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace YADE;

Application *Application::_Instance = nullptr;

Application::Application()
{
	_Instance = this;
}

Application::~Application()
{
	
}

void Application::Run()
{
	double elapsed = 0;

	LOG("YA3DE Alpha 1.0");
	LOG("Bootstrapping...");

	Initialize();
	CreateRenderWindow();
	ContentManager::Instance()->Dispatcher().InitializeAsyncWorkers();

	BOOST_FOREACH(boost::property_tree::iptree::value_type &v, _Settings.get_child("config.FileSystem"))
		FileManager::Instance()->LoadFileSystem(v.first, v.second.data());

	Load();

	_Running = true;
	while (_Running)
	{
		sf::Event e;
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		while (_Window.pollEvent(e))
			OnEvent(e, elapsed);

		if (!_Running)
			break;

		Update(elapsed);
		Render(elapsed);

		_Window.display();

		elapsed = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}

	Unload();

	ContentManager::Instance()->Dispatcher().Shutdown();
	_Window.close();
}

void Application::Initialize()
{
	LOG("Reading configuration file: ya3de.xml");
	boost::property_tree::read_xml("ya3de.xml", _Settings);
}

void Application::CreateRenderWindow()
{
	int wsstyle = sf::Style::Close | sf::Style::Titlebar;
	if (_Settings.get<bool>("config.window.fullscreen", false))
		wsstyle |= sf::Style::Fullscreen;

	sf::VideoMode videoMode;
	videoMode.width = _Settings.get<int>("config.window.width", 800);
	videoMode.height = _Settings.get<int>("config.window.height", 600);
	videoMode.bitsPerPixel = _Settings.get<int>("config.window.bitsperpixel", 32);

	sf::ContextSettings ctx;
	ctx.majorVersion = 3;
	ctx.minorVersion = 1;
	
	LOG("Creating RenderWindow");
	_Window.create(videoMode, _Settings.get<std::string>("config.window.title", "YA3DE - Yet Another 3D Engine"), wsstyle, ctx);
	_Window.setVerticalSyncEnabled(_Settings.get<bool>("config.window.vsync", true));

	LOG("Video mode: %dx%d, %dbpp [%s]", videoMode.width, videoMode.height, videoMode.bitsPerPixel, _Settings.get<bool>("config.window.fullscreen", false) ? "Fullscreen" : "Windowed");
	LOG("OpenGL version %d.%d", _Window.getSettings().majorVersion, _Window.getSettings().minorVersion);
	LOG("FSAA = %d", _Window.getSettings().antialiasingLevel);
	LOG("GL_VERSION = %s", glGetString(GL_VERSION));
	LOG("GL_VENDOR = %s", glGetString(GL_VENDOR));
	LOG("GL_RENDERER = %s", glGetString(GL_RENDERER));
	LOG("GL_EXTENSIONS = %s", glGetString(GL_EXTENSIONS));

	glewInit();
}

void Application::Load()
{

}

void Application::OnEvent(const sf::Event &e, double elapsed)
{

}

void Application::Update(double elapsed)
{

}

void Application::Render(double elapsed)
{

}

void Application::Unload()
{

}
