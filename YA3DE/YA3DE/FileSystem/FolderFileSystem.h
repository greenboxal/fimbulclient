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

#ifndef _FOLDEFILESYSTEM_H_
#define _FOLDEFILESYSTEM_H_

#include <exception>
#include <fstream>
#include <YA3DE/FileSystem/IFileSystem.h>
#include <YA3DE/FileSystem/IFile.h>

namespace YA3DE
{
	namespace FileSystem
	{
		class FolderFile : public IFile
		{
		public:
			FolderFile(const std::string &base, const std::string &name)
			{
#ifdef _WIN32
				_filename = base + '\\' + name;
#else
				_filename = base + '/' + name;
				for (int i = 0; i < _filename.length(); ++i)
					if (_filename[i] == '\\')
					  _filename[i] = '/';
#endif
			}

			virtual ~FolderFile()
			{
				if (_stream.is_open())
					Close();
			}

			virtual bool Open()
			{
				_stream.open(_filename, std::fstream::in | std::fstream::binary);
				return !_stream.fail();
			}

			virtual void Seek(int origin, int offset)
			{
				std::fstream::seekdir dir;

				if (origin == 0)
					dir = std::fstream::beg;
				else if (origin == 1)
					dir = std::fstream::cur;
				else if (origin == 2)
					dir = std::fstream::end;
				else
					throw std::exception("Invalid seek origin!");

				_stream.seekg(dir, offset);
			}

			virtual void Read(void *data, int size)
			{
				_stream.read((char *)data, size);
			}

			virtual char ReadByte()
			{
				char c = 0;

				_stream.read(&c, 1);

				return c;
			}

			virtual size_t GetSize()
			{
				std::streamoff size, old;

				old = _stream.tellg();
				_stream.seekg(0, std::ios::end);
				size = _stream.tellg();
				_stream.seekg(old);

				return (size_t)size;
			}

			virtual void Close()
			{
				_stream.close();
			}

		private:
			std::fstream _stream;
			std::string _filename;
		};
		
		class FolderFileSystem : public IFileSystem
		{
		public:
			FolderFileSystem(const std::string &base)
			{
				_base = base;
			}

			FilePtr Open(const std::string &filename)
			{
				FilePtr file((IFile *)new FolderFile(_base, filename));

				if (file->Open())
					return file;

				return NULL;
			}

		private:
			std::string _base;
		};

		class FolderFileSystemFactory : public IFileSystemFactory
		{
		public:
			virtual FileSystemPtr Create(const std::string &path)
			{
				return FileSystemPtr(new FolderFileSystem(path));
			}
		};
	}
}

#endif
