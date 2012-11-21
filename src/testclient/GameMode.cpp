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

#include "GameMode.h"
#include "RagnarokClient.h"

using namespace testclient;
using namespace YADE;

GameMode::GameMode()
{
	_ActiveSubMode = nullptr;
}

GameMode::~GameMode()
{

}

void GameMode::Load()
{

}

void GameMode::OnEvent(const sf::Event &e, double elapsed)
{
	if (_ActiveSubMode)
		_ActiveSubMode->OnEvent(e, elapsed);
}

void GameMode::Update(double elapsed)
{
	if (_ActiveSubMode)
		_ActiveSubMode->Update(elapsed);
}

void GameMode::Render(double elapsed)
{
	if (_ActiveSubMode)
		_ActiveSubMode->Render(elapsed);
}

void GameMode::Unload()
{
	std::map<size_t, SubGameMode *>::iterator it;
	for (it = _SubGameModes.begin(); it != _SubGameModes.end(); it++)
		delete it->second;
}

void GameMode::ChangeSubMode(size_t mode)
{
	std::map<size_t, SubGameMode *>::iterator it = _SubGameModes.find(mode);

	if (it == _SubGameModes.end())
		return;

	if (_ActiveSubMode)
		_ActiveSubMode->Unload();

	_ActiveSubMode = it->second;
	_ActiveSubMode->Load();

	RagnarokInstance->Gui()->View()->set_js_method_handler(_ActiveSubMode);
}

void GameMode::RegisterSubMode(size_t modeId, SubGameMode *mode)
{
	_SubGameModes[modeId] = mode;
}
