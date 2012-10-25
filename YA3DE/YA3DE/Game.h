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

#pragma once

#include <YA3DE/Helpers.h>
#include <YA3DE/System/RenderWindow.h>

#include <memory>
#include <boost/property_tree/ptree.hpp>

namespace YA3DE
{
	class Game : public std::enable_shared_from_this<Game>
	{
	public:
		Game();
		virtual ~Game();

		void Run();
		void Exit()
		{
			_Running = false;
		}
		
		static Game *Instance()
		{
			return _instance;
		}

		DEFPROP_RO(public, bool, Running);
		DEFPROP_P_RO(public, boost::property_tree::iptree, Settings);
		DEFPROP_RO(public, YA3DE::System::RenderWindow, Window);
		DEFPROP_RO(public, double, FrameRate);
		DEFPROP_RO(public, bool, IsActive);

	protected:
		virtual void OnInitialize() = 0;
		virtual void OnLoad() = 0;
		virtual void OnUpdate(double elapsedTime) = 0;
		virtual void OnRender(double elapsedTime) = 0;
		virtual void OnUnload() = 0;

		virtual void OnEvent(YA3DE::System::Event &ev, double elapsedTime) { }

	private:
		void ReadConfig();

		static Game *_instance;
	};
}

