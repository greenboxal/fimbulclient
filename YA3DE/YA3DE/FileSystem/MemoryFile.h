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

#ifndef _MEMORYFILE_H_
#define _MEMORYFILE_H_

#include <memory.h>

#include <YA3DE/FileSystem/IFile.h>

namespace YA3DE
{
	namespace FileSystem
	{
		class MemoryFile : public IFile
		{
		public:
			MemoryFile(char *data, int size)
				: _data(data), _size(size), _offset(0)
			{

			}

			virtual ~MemoryFile()
			{
				Close();
			}

			virtual bool Open()
			{
				return true;
			}

			virtual void Seek(int origin, int offset)
			{
				int x = 0;

				if (origin == 0)
					x = 0;
				else if (origin == 1)
					x = _offset;
				else if (origin == 2)
					x = _size;

				_offset = x + offset;
			}

			virtual void Read(const void *data, int size)
			{
				memcpy((void *)data, &_data[_offset], size);
				_offset += size;
			}

			virtual char ReadByte()
			{
				return _data[_offset++];
			}

			virtual size_t GetSize()
			{
				return _size;
			}

			virtual void Close()
			{
				if (_data)
				{
					delete[] _data;
					_data = NULL;
				}
			}

			DEFPROP_I_RO(public, size_t, Size, GetSize);

		private:
			char *_data;
			size_t _size;
			size_t _offset;
		};
	}
}

#endif
