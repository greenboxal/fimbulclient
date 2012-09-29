/*	This file is part of FimbulwinterClient.

	FimbulwinterClient is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FimbulwinterClient.  If not, see <http://www.gnu.org/licenses/>. */

#include <unordered_map>
#include <fstream>
#include <SFML/System.hpp>
#include <YA3DE/Helpers.h>
#include <YA3DE/Content/ContentName.h>

namespace ROFileSystem
{
	namespace Grf
	{
		struct GrfFile
		{
		public:
			GrfFile()
			{

			}

			GrfFile(unsigned int len, unsigned int ulen, unsigned int alen, unsigned char flags, unsigned int offset)
				: _CompressedLength(len), _UncompressedLength(ulen), _CompressedLengthAligned(alen), _Flags(flags), _Offset(offset)
			{
			}

			DEFPROP_RO(public, unsigned int, CompressedLength);
			DEFPROP_RO(public, unsigned int, UncompressedLength);
			DEFPROP_RO(public, unsigned int, CompressedLengthAligned);
			DEFPROP_RO(public, unsigned char, Flags);
			DEFPROP_RO(public, unsigned int, Offset);
		};

		class Grf
		{
		public:
			typedef YA3DE::Content::ContentNameTraits<GrfFile>::HashTable GrfFileMap;

			bool Open(const std::string &filename);
			GrfFile *FindFile(const std::string &filename);
			void *GetFileData(GrfFile *file, size_t &size);
			void Close();

			DEFPROP_RO_R(public, GrfFileMap, Files);

		private:
			sf::Mutex _mutex;
			std::ifstream _stream;
		};
	}
}
