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

#include <YA3DE/Content/StringResource.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>

using namespace YADE;

template<>
std::shared_ptr<StringResource> ContentManager::LoadNew(const std::string &filename, bool async)
{
	FilePtr fp = FileManager::Instance()->OpenFile(filename);

	if (!fp)
		return NULL;

	int size = fp->GetSize();
	unsigned char *data = new unsigned char[size + 1];
	fp->Read((char *)data, size);
	fp->Close();

	data[size] = 0;

	return std::make_shared<StringResource>(data, size + 1);
}
