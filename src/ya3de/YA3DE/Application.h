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

#ifndef YA3DE_APPLICATION_H
#define YA3DE_APPLICATION_H

#include <boost/property_tree/ptree.hpp>
#include <YA3DE/Helpers.h>
#include <SFML/Graphics.hpp>

namespace YADE
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		bool IsActive()
		{
			return _Active;
		}

		static Application *Instance()
		{
			return _Instance;
		}

		DEFPROP_P_RO_NR(public, sf::RenderWindow, Window);
		DEFPROP_P_RO_NR(public, boost::property_tree::iptree, Settings);

	protected:
		virtual void Initialize();
		virtual void Load();
		virtual void OnEvent(const sf::Event &e, double elapsed);
		virtual void Update(double elapsed);
		virtual void Render(double elapsed);
		virtual void Unload();

	private:
		bool _Running;
		bool _Active;

		void CreateRenderWindow();

		static Application *_Instance;
	};
}

#endif
