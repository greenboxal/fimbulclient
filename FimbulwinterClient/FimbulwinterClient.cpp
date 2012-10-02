/*	This file is part of FimbulwinterClient.

	FimbulwinterClient is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FimbulwinterClient.  If not, see <http://www.gnu.org/licenses/>. */

#include "FimbulwinterClient.h"
#include "FileSystem/GrfFileSystem.h"

#include <YA3DE/Graphics/Font.h>
#include <YA3DE/Graphics/Graphics2D.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>
#include <YA3DE/GUI/GUIManager.h>

#include "GUI/Renderers/ControlRenderer.h"
#include "GUI/Renderers/LabelRenderer.h"

#include "WorldGameMode.h"

using namespace YA3DE::Content;
using namespace YA3DE::Graphics;
using namespace YA3DE::GUI;
using namespace YA3DE::FileSystem;

using namespace ROFileSystem;
using namespace ROGUI::Renderers;

void ClientGame::OnInitialize()
{
	FileManager::Instance()->RegisterFileSystemFactory("Grf", FileSystemFactoryPtr(new GrfFileSystemFactory()));

	Configuration = new Config("ragnarok.xml");
	Configuration->Read();
}

void ClientGame::OnLoad()
{
	ChangeGameMode(new WorldGameMode());
}

void ClientGame::OnUpdate(double elapsedTime)
{
	if (_Mode != NULL)
		_Mode->Update(elapsedTime);
}

void ClientGame::OnRender(double elapsedTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (_Mode != NULL)
		_Mode->Render(elapsedTime);
}

void ClientGame::OnUnload()
{
	ChangeGameMode(NULL);
}

void ClientGame::OnEvent(sf::Event &ev, double elapsedTime)
{
	if (ev.type == sf::Event::Closed)
	{
		Running = false;
		return;
	}
	
	if (_Mode != NULL)
		_Mode->DispatchEvent(ev, elapsedTime);
}

int main(int argc, char **argv)
{
	std::shared_ptr<ClientGame> game(new ClientGame());

	game->Run();
	
	return 0;
}

