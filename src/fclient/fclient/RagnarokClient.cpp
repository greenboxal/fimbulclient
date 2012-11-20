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

#include <fclient/RagnarokClient.h>
#include <fclient/TestGameMode.h>
#include <fclient/FileSystem/GrfFileSystem.h>

#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/FileSystem/FileManager.h>

#include <boost/property_tree/xml_parser.hpp>

using namespace fclient;
using namespace YADE;
using namespace Awesomium;

RagnarokClient::RagnarokClient()
{
	_CurrentGameMode = nullptr;
}

RagnarokClient::~RagnarokClient()
{
}

void RagnarokClient::ChangeGameMode(GameMode *mode)
{
	if (_CurrentGameMode != nullptr)
	{
		_CurrentGameMode->Unload();
		delete _CurrentGameMode;
	}

	_CurrentGameMode = mode;

	if (_CurrentGameMode != nullptr)
		_CurrentGameMode->Load();
}

void RagnarokClient::Initialize()
{
	LOG("Reading configuration file: ragnarok.xml");
	boost::property_tree::read_xml("ragnarok.xml", _Settings);

	FileManager::Instance()->RegisterFileSystemFactory("Grf", std::make_shared<GrfFileSystemFactory>());
}

void RagnarokClient::Load()
{
	_Gui = new GuiManager(_Window);
	_Gui->Load();

	ChangeGameMode(new TestGameMode());
}

void RagnarokClient::OnEvent(const sf::Event &e, double elapsed)
{
	if (!_Gui->DispatchEvent(e) && _CurrentGameMode != nullptr)
		_CurrentGameMode->OnEvent(e, elapsed);
}

void RagnarokClient::Update(double elapsed)
{
	_Gui->Update();

	if (_CurrentGameMode != nullptr)
		_CurrentGameMode->Update(elapsed);
}

void RagnarokClient::Render(double elapsed)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (_CurrentGameMode != nullptr)
		_CurrentGameMode->Render(elapsed);

	_Gui->Render();
}

void RagnarokClient::Unload()
{
	ChangeGameMode(nullptr);

	_Gui->Unload();
}
