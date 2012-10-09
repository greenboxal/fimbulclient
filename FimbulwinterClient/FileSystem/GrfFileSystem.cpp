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

#include "GrfFileSystem.h"
#include <malloc.h>

using namespace ROFileSystem;

bool GrfFileSystem::Load()
{
	return _grf.Open(_filename);
}

FilePtr GrfFileSystem::Open(const std::string &filename)
{
	Grf::GrfFile *file = _grf.FindFile(filename);
	
	if (!file)
		return NULL;

	size_t size = 0;
	void *data = _grf.GetFileData(file, size);

	if (!data)
		return NULL;

	return FilePtr(new MemoryFile((char *)data, size));
}
