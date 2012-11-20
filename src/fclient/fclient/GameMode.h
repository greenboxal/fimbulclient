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

namespace fclient
{
	class SubGameMode
	{
	public:
		virtual ~SubGameMode() { }
		virtual void Load() = 0;
		virtual void OnEvent(const sf::Event &e, double elapsed) = 0;
		virtual void Update(double elapsed) = 0;
		virtual void Render(double elapsed) = 0;
		virtual void Unload() = 0;
	};

	class GameMode
	{
	public:
		GameMode()
		{
			_ActiveSubMode = nullptr;
		}

		virtual ~GameMode()
		{

		}

		virtual void Load()
		{

		}

		virtual void OnEvent(const sf::Event &e, double elapsed)
		{
			if (_ActiveSubMode)
				_ActiveSubMode->OnEvent(e, elapsed);
		}

		virtual void Update(double elapsed)
		{
			if (_ActiveSubMode)
				_ActiveSubMode->Update(elapsed);
		}

		virtual void Render(double elapsed)
		{
			if (_ActiveSubMode)
				_ActiveSubMode->Render(elapsed);
		}

		virtual void Unload()
		{
			std::map<size_t, SubGameMode *>::iterator it;
			for (it = _SubGameModes.begin(); it != _SubGameModes.end(); it++)
				delete it->second;
		}

		void ChangeSubMode(size_t mode)
		{
			std::map<size_t, SubGameMode *>::iterator it = _SubGameModes.find(mode);

			if (it == _SubGameModes.end())
				return;

			if (_ActiveSubMode)
				_ActiveSubMode->Unload();

			_ActiveSubMode = it->second;;
			_ActiveSubMode->Load();
		}

	protected:
		void RegisterSubMode(size_t modeId, SubGameMode *mode)
		{
			_SubGameModes[modeId] = mode;
		}

	private:
		std::map<size_t, SubGameMode *> _SubGameModes;
		SubGameMode *_ActiveSubMode;
	};
}

#endif
