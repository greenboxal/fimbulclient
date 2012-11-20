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

#ifndef FCLIENT_TESTGAMEMODE_H
#define FCLIENT_TESTGAMEMODE_H

#include <fclient/GameMode.h>

namespace fclient
{
	class TestSubGameMode : public SubGameMode
	{
	public:
		TestSubGameMode();
		virtual ~TestSubGameMode();

		virtual void Load();
		virtual void OnEvent(const sf::Event &e, double elapsed);
		virtual void Update(double elapsed);
		virtual void Render(double elapsed);
		virtual void Unload();
	};

	class TestGameMode : public GameMode
	{
	public:
		virtual void Load()
		{
			RegisterSubMode(0, new TestSubGameMode());
			ChangeSubMode(0);
		}
	};
}

#endif
