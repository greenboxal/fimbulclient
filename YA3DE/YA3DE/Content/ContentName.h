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

#pragma once

#include <string>
#include <unordered_map>

#include <YA3DE/Helpers.h>

namespace YA3DE
{
	namespace Content
	{
		class ContentName
		{
		public:
			ContentName(const char *name)
			{
				_Name = name;
				_Hash = MakeHash(name);
			}

			ContentName(const std::string &name)
			{
				_Name = name;
				_Hash = MakeHash(name);
			}

			bool operator==(const ContentName &other) const
			{
				return other._Hash == _Hash;
			}
			
			DEFPROP_RO(public, unsigned int, Hash);
			DEFPROP_RO_R(public, std::string, Name);

		private:
			static unsigned int MakeHash(const std::string &s);
		};

		struct ContenNameHasher
		{
		public:
			size_t operator()(const ContentName &cn) const
			{
				return cn.Hash;
			}
		};

		template<typename _Ty>
		class ContentHashTable : public std::unordered_map<ContentName, _Ty, ContenNameHasher>
		{

		};
	}
}

