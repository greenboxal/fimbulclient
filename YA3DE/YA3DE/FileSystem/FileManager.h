/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <string>
#include <list>
#include <map>
#include <YA3DE/Helpers.h>
#include <YA3DE/FileSystem/IFileSystem.h>
#include <YA3DE/FileSystem/IFile.h>
#include <YA3DE/FileSystem/FolderFileSystem.h>

namespace YA3DE
{
	namespace FileSystem
	{
		class FileManager
		{
		public:
			FileManager()
			{
				if (_instance != NULL)
					throw std::exception("YA3DE::FileSystem::FileManager must have only one instance.");

				RegisterFileSystemFactory("Folder", FileSystemFactoryPtr(new FolderFileSystemFactory()));

				_instance = this;
			}

			FilePtr OpenFile(const std::string &name)
			{
				std::list<FileSystemPtr>::iterator it;
				for (it = _fileSystems.begin(); it != _fileSystems.end(); it++)
				{
					FilePtr file;

					if ((file = (*it)->Open(name)) != NULL)
						return file->shared_from_this();
				}

				return NULL;
			}

			void RegisterFileSystem(FileSystemPtr fileSystem)
			{
				_fileSystems.push_back(fileSystem);
			}

			void RegisterFileSystemFactory(const std::string &name, FileSystemFactoryPtr factory)
			{
				if (_fileSystemFactories.count(name))
					throw std::exception("There are already a FileSystemFactory with this name.");

				_fileSystemFactories[name] = factory;
			}

			void LoadFileSystem(const std::string &name, const std::string &path)
			{
				std::map<std::string, FileSystemFactoryPtr>::iterator it;
				for (it = _fileSystemFactories.begin(); it != _fileSystemFactories.end(); it++)
				{
					if (it->first == name)
					{
						FileSystemPtr fs;

						if ((fs = it->second->Create(path)) != NULL)
							RegisterFileSystem(fs);

						break;
					}
				}
			}

			static FileManager *Instance()
			{
				if (_instance == NULL)
					_instance = new FileManager();

				return _instance;
			}
		private:
			std::list<FileSystemPtr> _fileSystems;
			std::map<std::string, FileSystemFactoryPtr> _fileSystemFactories;

			static FileManager *_instance;
		};
	}
}

#endif
