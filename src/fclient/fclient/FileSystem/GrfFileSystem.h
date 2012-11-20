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

#ifndef FCLIENT_GRF_FILESYSTEM_H
#define FCLIENT_GRF_FILESYSTEM_H

#include <fclient/FileSystem/Grf/Grf.h>
#include <YA3DE/FileSystem/IFileSystem.h>
#include <YA3DE/FileSystem/MemoryFile.h>

namespace fclient
{
	using namespace YADE;

	class GrfFileSystem : public IFileSystem
	{
	public:
		GrfFileSystem(const std::string &filename)
		{
			_filename = filename;
		}
		
		bool Load();
		FilePtr Open(const std::string &filename);

	private:
		Grf::Grf _grf;
		std::string _filename;
	};

	class GrfFileSystemFactory : public IFileSystemFactory
	{
	public:
		virtual FileSystemPtr Create(const std::string &path)
		{
			GrfFileSystem *grf = new GrfFileSystem(path);

			if (!grf->Load())
			{
				delete grf;
				return NULL;
			}

			return FileSystemPtr(grf);
		}
	};
}

#endif
