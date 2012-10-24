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

#include <string>
#include <YA3DE/Graphics/Graphics2D.h>
#include <YA3DE/GUI/Control.h>

using namespace YA3DE::Graphics;

namespace YA3DE
{
	namespace GUI
	{
		class IGUIRenderer
		{
		public:
			virtual void Render(Graphics2D &g) = 0;
		};

		class IGUIRendererFactory
		{
		public:
			virtual std::string GetName() = 0;
			virtual IGUIRenderer *CreateRenderer(Control *control) = 0;
		};
	}
}

