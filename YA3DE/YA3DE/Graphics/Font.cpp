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

#include <YA3DE/Game.h>
#include <YA3DE/Math.h>
#include <YA3DE/Graphics/Font.h>
#include <YA3DE/Graphics/Texture2D.h>
#include <YA3DE/FileSystem/FileManager.h>

using namespace YA3DE;
using namespace YA3DE::Content;
using namespace YA3DE::Graphics;
using namespace YA3DE::FileSystem;

Font::Font(std::string name, int size, FontStyle style)
{
	
}

void Font::Render(std::string text, glm::uvec2 pos, glm::vec4 color)
{
	
}

int Font::Measure(std::string text)
{
	return 0;
}
