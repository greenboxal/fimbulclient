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

#ifndef _IFILE_H_
#define _IFILE_H_

#include <memory>

#include <YA3DE/Helpers.h>

namespace YA3DE
{
	namespace FileSystem
	{
		class IFile : public std::enable_shared_from_this<IFile>
		{
		public:
			virtual ~IFile() { }

			virtual bool Open() = 0;
			virtual void Seek(int origin, int offset) = 0;
			virtual void Read(const void *data, int size) = 0;
			virtual char ReadByte() = 0;
			virtual size_t GetSize() = 0;
			virtual void Close() = 0;
		};
		typedef std::shared_ptr<IFile> FilePtr;
	}
}

#endif
