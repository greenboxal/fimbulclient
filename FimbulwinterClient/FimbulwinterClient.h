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

#ifndef _FIMBULWINTERCLIENT_H_
#define _FIMBULWINTERCLIENT_H_

#include <YA3DE/Game.h>
#include <YA3DE/System/Event.h>

#include "GameMode.h"

using namespace YA3DE;

#define Ragnarok ((ClientGame *)Game::Instance())

class ClientGame : public Game
{
public:
	ClientGame()
	{
		_Mode = NULL;
	}

	~ClientGame()
	{
		if (_Mode != NULL)
		{
			_Mode->OnUnload();
			delete _Mode;
		}
	}

	void ChangeGameMode(GameMode *mode)
	{
		if (_Mode != NULL)
		{
			_Mode->OnUnload();
			delete _Mode;
		}
		
		_Mode = mode;

		if (_Mode != NULL)
			_Mode->OnLoad();
	}

	DEFPROP_RO(public, GameMode *, Mode);

protected:
	virtual void OnInitialize();
	virtual void OnLoad();
	virtual void OnUpdate(double elapsedTime);
	virtual void OnRender(double elapsedTime);
	virtual void OnUnload();
	
	virtual void OnEvent(YA3DE::System::Event &ev, double elapsedTime);
};

#endif
