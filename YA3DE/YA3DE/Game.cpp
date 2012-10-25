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

#include <stdafx.h>
#include <YA3DE/Game.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>

using namespace YA3DE;
using namespace YA3DE::Content;
using namespace YA3DE::FileSystem;
using namespace YA3DE::System;

Game *Game::_instance = NULL;

Game::Game()
{
	if (_instance != NULL)
		throw std::exception("YA3DE::Game must have only one instance.");
	
	// We must ensure ContentManager creation at the main thread
	new ContentManager();

	_instance = this;
}

Game::~Game()
{
}

void Game::ReadConfig()
{
	// Create the Window
	int wsstyle = WindowStyle::CloseButton | WindowStyle::Titlebar;

	if (_Settings.get<bool>("config.window.fullscreen", false))
		wsstyle |= WindowStyle::FullScreen;

	LOG("Creating RenderWindow");
	_Window.Create(VideoMode(_Settings.get<int>("config.window.width", 800), _Settings.get<int>("config.window.height", 600), _Settings.get<int>("config.window.bitsperpixel", 32)), wsstyle);
	_Window.SetVSync(_Settings.get<bool>("config.window.vsync", true));
	_Window.SetTitle(_Settings.get<std::string>("config.window.title", "YA3DE - Yet Another 3D Engine"));

	LOG("Video mode: %dx%d, %dbpp [%s]", _Window.Mode.Width, _Window.Mode.Height, _Window.Mode.BitsPerPixel, _Window.Mode.FullScreen ? "Fullscreen" : "Windowed");
	LOG("OpenGL version %d.%d", _Window.Mode.GLMajor, _Window.Mode.GLMinor);
	LOG("FSAA = %d", _Window.Mode.AASamples);
	LOG("GL_VERSION = %s", glGetString(GL_VERSION));
	LOG("GL_VENDOR = %s", glGetString(GL_VENDOR));
	LOG("GL_RENDERER = %s", glGetString(GL_RENDERER));
	LOG("GL_EXTENSIONS = %s", glGetString(GL_EXTENSIONS));

	// Initialize FileSystem
	BOOST_FOREACH(boost::property_tree::iptree::value_type &v, _Settings.get_child("config.FileSystem"))
		FileManager::Instance()->LoadFileSystem(v.first, v.second.data());

	/*rapidxml::xml_node<> *config = _Configuration->GetRoot().first_node("Config");

	rapidxml::xml_node<> *window = config->first_node("Window");
	if (window)
	{
		int style = WindowStyle::CloseButton | WindowStyle::Titlebar;
		std::string title = window->first_attribute("Title")->value();

		if (stricmp(window->first_attribute("Fullscreen")->value(), "true") == 0)
			style |= WindowStyle::FullScreen;
		
		LOG("Creating RenderWindow");
		_Window.Create(VideoMode(atoi(window->first_attribute("Width")->value()), atoi(window->first_attribute("Height")->value()), atoi(window->first_attribute("BitsPerPixel")->value())), style);
		_Window.SetVSync(stricmp(window->first_attribute("VSync")->value(), "true") == 0);

		LOG("Video mode: %dx%d, %dbpp [%s]", _Window.Mode.Width, _Window.Mode.Height, _Window.Mode.BitsPerPixel, _Window.Mode.FullScreen ? "Fullscreen" : "Windowed");
		LOG("OpenGL version %d.%d", _Window.Mode.GLMajor, _Window.Mode.GLMinor);
		LOG("FSAA = %d", _Window.Mode.AASamples);
		LOG("GL_VERSION = %s", glGetString(GL_VERSION));
		LOG("GL_VENDOR = %s", glGetString(GL_VENDOR));
		LOG("GL_RENDERER = %s", glGetString(GL_RENDERER));
		LOG("GL_EXTENSIONS = %s", glGetString(GL_EXTENSIONS));
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
	}*/
}

void Game::Run()
{
	double total = 0, frames = 0, elapsed = 0;

	LOG("YA3DE Alpha");

	LOG("Bootstraping");
	OnInitialize();

	ReadConfig();

	_Window.MainContext->UnbindCurrent();
	ContentManager::Instance()->Dispatcher.InitializeAsyncWorkers();
	_Window.MainContext->MakeCurrent();
	
	LOG("Loading");
	OnLoad();

	_Running = true;
	
	LOG("Entering main loop");
	while (_Running)
	{
		Event ev;
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		while(_Window.PollEvent(ev) && _Running)
			OnEvent(ev, elapsed);

		if (!_Running)
			break;

		OnUpdate(elapsed);
		OnRender(elapsed);

		total += elapsed;
		frames++;

		if (total > 1000.0F)
		{
			_FrameRate = frames;
			frames = 0;
			total -= 1000.0F;
		}

		_Window.EndScene();

		elapsed = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}
	
	LOG("Unloading");
	OnUnload();
	
	ContentManager::Instance()->Dispatcher.Shutdown();

	LOG("Closed");
	_Window.Close();
}
