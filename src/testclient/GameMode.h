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

#ifndef FCLIENT_GAMEMODE_H
#define FCLIENT_GAMEMODE_H

#include <map>
#include <YA3DE/Application.h>
#include <Awesomium/WebCore.h>

namespace testclient
{
	class GameMode
	{
	public:
		GameMode() { }
		virtual ~GameMode() { }

		virtual void Load() = 0;
		virtual void OnEvent(const sf::Event &e, double elapsed) = 0;
		virtual void Update(double elapsed) = 0;
		virtual void Render(double elapsed) = 0;
		virtual void Unload() = 0;
	};
}

#endif
