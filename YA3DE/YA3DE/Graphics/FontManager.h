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

#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include <list>
#include <YA3DE/Helpers.h>
#include <YA3DE/Graphics/Font.h>

namespace YA3DE
{
	namespace Graphics
	{
		class FontManager
		{
			FontPtr LoadFont(std::string name, int size, FontStyle style)
			{
				std::list<FontPtr>::iterator it;
				for (it = _instances.begin(); it != _instances.end(); it++)
					if ((*it)->Name() == name && (*it)->Size() == size && (*it)->Style() == style)
						break;

				if (it != _instances.end())
					return *it;

				FontPtr ptr = std::make_shared<Font>(name, size, style);
				_instances.push_back(ptr);
				return ptr;
			}

			static FontManager *Instance()
			{
				if (_Instance == NULL)
					_Instance = new FontManager();

				return _Instance;
			}
		public:
			std::list<FontPtr> _instances;
			static FontManager *_Instance;
		};
	}
}

#endif
