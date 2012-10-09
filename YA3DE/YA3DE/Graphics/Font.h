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

#ifndef _FONT_H_
#define _FONT_H_

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>

#include <SFML/Graphics.hpp>

namespace YA3DE
{
	namespace Graphics
	{
		enum FontStyle
		{
			None = 0x0,
			Bold = 0x1,
			Italic = 0x2,
			Underline = 0x4,
			Strikeout = 0x8,
		};

		class Font : public std::shared_ptr<Font>
		{
		public:
			Font(std::string name, int size, FontStyle style);
			~Font();

			void Render(std::string text, glm::uvec2 pos, glm::vec4 color);
			int Measure(std::string text);
			
			DEFPROP_RO(public, std::string, Name);
			DEFPROP_RW(public, int, Size);
			DEFPROP_RO(public, int, Height);
			DEFPROP_RO(public, FontStyle, Style);
			
			DEFPROP_SELF_RO(public, bool, Bold)
			{
				return (_Style & FontStyle::Bold) != 0;
			}

			DEFPROP_SELF_RO(public, bool, Italic)
			{
				return (_Style & FontStyle::Italic) != 0;
			}

			DEFPROP_SELF_RO(public, bool, Underline)
			{
				return (_Style & FontStyle::Underline) != 0;
			}

			DEFPROP_SELF_RO(public, bool, Strikeout)
			{
				return (_Style & FontStyle::Strikeout) != 0;
			} 
		};
		typedef std::shared_ptr<Font> FontPtr;
	}
}

#endif
