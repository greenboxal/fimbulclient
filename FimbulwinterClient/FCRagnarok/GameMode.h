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

#pragma once

#include <vector>

#include <YA3DE/Helpers.h>
#include <YA3DE/System/Event.h>

class SubGameMode
{
public:
	virtual ~SubGameMode()
	{

	}

	virtual void OnLoad() = 0;
	virtual void Update(double elapsed) = 0;
	virtual void Render(double elapsed) = 0;
	virtual void OnEvent(YA3DE::System::Event &ev, double elapsedTime) = 0;
	virtual void OnUnload() = 0;
};

class GameMode
{
public:
	GameMode()
	{
		_SubMode = -1;
	}

	virtual ~GameMode()
	{
		if (_SubMode != -1)
			_subGameModes[_SubMode]->OnUnload();

		std::vector<SubGameMode *>::iterator it;
		for (it = _subGameModes.begin(); it != _subGameModes.end(); it++)
			delete *it;
	}

	virtual void OnLoad() = 0;
	virtual void OnUnload() = 0;

	void Update(double elapsed)
	{
		if (_SubMode != -1)
			_subGameModes[_SubMode]->Update(elapsed);
	}

	void Render(double elapsed)
	{
		if (_SubMode != -1)
			_subGameModes[_SubMode]->Render(elapsed);
	}

	void DispatchEvent(YA3DE::System::Event &ev, double elapsedTime)
	{
		if (_SubMode != -1)
			_subGameModes[_SubMode]->OnEvent(ev, elapsedTime);
	}

	void ChangeSubMode(int id)
	{
		if (id < -1 || (unsigned int)id > _subGameModes.size())
			return;

		if (_SubMode != -1)
			_subGameModes[_SubMode]->OnUnload();

		_SubMode = id;
		
		if (_SubMode != -1)
			_subGameModes[_SubMode]->OnLoad();
	}

	DEFPROP_RO(public, int, SubMode);

protected:
	std::vector<SubGameMode *> _subGameModes;
};

