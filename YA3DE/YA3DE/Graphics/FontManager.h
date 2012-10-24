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

#include <map>
#include <list>
#include <YA3DE/Helpers.h>
#include <YA3DE/Graphics/Font.h>

namespace YA3DE
{
	namespace Graphics
	{
		class FontManager
		{
		public:
			FontManager()
			{
				_Instance = this;
			}

			FontPtr LoadFont(const std::string &name, int size, int style = FontStyle::None, FontLoadPolicy policy = FontLoadPolicy::Direct);

			static FontManager *Instance()
			{
				if (_Instance == NULL)
					new FontManager();

				return _Instance;
			}
		public:
			std::map<std::string, std::list<std::weak_ptr<Font> > > _Instances;
			static FontManager *_Instance;
		};
	}
}

