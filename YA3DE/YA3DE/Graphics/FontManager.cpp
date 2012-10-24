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

#include <stdafx.h>
#include <YA3DE/Graphics/FontManager.h>
#include <YA3DE/FileSystem/FileManager.h>

using namespace YA3DE::Graphics;
using namespace YA3DE::FileSystem;

FontManager *FontManager::_Instance;

FontPtr FontManager::LoadFont(const std::string &name, int size, int style, FontLoadPolicy policy)
{
	std::list<std::weak_ptr<Font> > &cache = _Instances[name];
	std::list<std::weak_ptr<Font> >::iterator it;
	
	for (it = cache.begin(); it != cache.end(); it++)
	{
		if (it->expired())
		{
			cache.erase(it);
			continue;
		}
		
		FontPtr ptr = it->lock();
		if (ptr->Style == style && ptr->Size == size)
			return ptr;
	}

	FilePtr stream = FileManager::Instance()->OpenFile("data/fonts/" + name + ".ttf");
	unsigned int sz = stream->GetSize();
	char *data = new char[sz];
	stream->Read(data, stream->GetSize());
	stream->Close();

	FontPtr font = std::make_shared<Font>(data, sz, size, style, policy);
	_Instances[name].push_back(font);

	return font;
}
