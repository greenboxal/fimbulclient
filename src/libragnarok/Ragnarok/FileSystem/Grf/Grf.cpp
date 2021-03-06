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

#include <YA3DE/Helpers.h>
#include <Ragnarok/FileSystem/Grf/Grf.h>
#include <Ragnarok/FileSystem/Grf/GrfCrypt.h>
#include <Ragnarok/FileSystem/zlib/zlib.h>

#include <memory.h>

#define GRF_MAGIC "Master of Magic"

#include <YA3DE/Pack1.h>
struct GrfHeader
{
	char Magic[16];
	char EncryptionKey[14];
	unsigned int FileTableOffset;
	unsigned int M1;
	unsigned int M2;
	unsigned int Version;
} STRUCT_PACKED;
#include <YA3DE/PackEnd.h>

using namespace Ragnarok::Grf;

bool Grf::Open(const std::string &filename)
{
	GrfHeader header;

	if (_Stream.is_open())
		return false;

	_Stream.open(filename, std::fstream::in | std::fstream::binary);

	if (_Stream.fail())
		return false;

	_Stream.read((char *)&header, sizeof(GrfHeader));

	if (_Stream.fail())
	{
		_Stream.close();
		return false;
	}

	if (memcmp(header.Magic, GRF_MAGIC, sizeof(header.Magic)) != 0)
	{
		_Stream.close();
		return false;
	}

	_Stream.seekg(header.FileTableOffset, std::fstream::cur);

	if (header.Version >= 0x200)
	{
		unsigned int fileTableLen = 0, fileTableULen = 0;

		_Stream.read((char *)&fileTableLen, sizeof(unsigned int));
		_Stream.read((char *)&fileTableULen, sizeof(unsigned int));

		if (_Stream.fail())
		{
			_Stream.close();
			return false;
		}

		char *compressed = new char[fileTableLen];
		char *uncompressed = new char[fileTableULen];

		_Stream.read(compressed, fileTableLen);

		if (_Stream.fail())
		{
			_Stream.close();
			delete[] compressed;
			delete[] uncompressed;
			return false;
		}

		if (uncompress((Bytef *)uncompressed, (uLongf *)&fileTableULen, (Bytef *)compressed, fileTableLen) != Z_OK)
		{
			_Stream.close();
			delete[] compressed;
			delete[] uncompressed;
			return false;
		}

		delete[] compressed;

		char *pointer = uncompressed;
		int fileCount = header.M2 - header.M1 - 7;
		for (int i = 0; i < fileCount; i++)
		{
			char *name = pointer;
			pointer += strlen(name) + 1;

#include <YA3DE/Pack1.h>
			struct
			{
				unsigned int len;
				unsigned int alen;
				unsigned int ulen;
				unsigned char flags;
				unsigned int offset;
			} STRUCT_PACKED tmp;
#include <YA3DE/PackEnd.h>
			
			memcpy(&tmp, pointer, sizeof(tmp));
			pointer += sizeof(tmp);

			if (tmp.flags & FILELIST_TYPE_FILE)
			{
				GrfFile file(tmp.len, tmp.ulen, tmp.alen, tmp.flags, tmp.offset);
				_Files[name] = file;
			}
		}

		delete[] uncompressed;
	}
	else
		return false;

	return true;
}

GrfFile *Grf::FindFile(const std::string &filename)
{
	GrfFileMap::iterator it = _Files.find(filename);

	if (it != _Files.end())
		return &it->second;

	return NULL;
}

void *Grf::GetFileData(GrfFile *file, size_t &size)
{
	char *compressed = new char[file->CompressedLengthAligned()];

	_Guard.lock();
	{
		_Stream.seekg(sizeof(GrfHeader) + file->Offset(), std::fstream::beg);

		if (_Stream.fail())
		{
			delete[] compressed;
			return NULL;
		}

		_Stream.read(compressed, file->CompressedLengthAligned());
		
		if (_Stream.fail())
		{
			delete[] compressed;
			return NULL;
		}
	}
	_Guard.unlock();

	GRF_Decode((unsigned char *)compressed, file->CompressedLengthAligned(), file->Flags(), file->CompressedLength());

	int nlen = file->UncompressedLength();
	char *uncompressed = new char[file->UncompressedLength()];
	if (uncompress((Bytef *)uncompressed, (uLongf *)&nlen, (const Bytef *)compressed, (uLong)file->CompressedLength()) != Z_OK)
	{
		delete[] compressed;
		delete[] uncompressed;
		return NULL;
	}

	size = file->UncompressedLength();

	delete[] compressed;
	return uncompressed;
}

void Grf::Close()
{
	if (_Stream.is_open())
		_Stream.close();
}
