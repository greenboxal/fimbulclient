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

#ifndef _CONTENTNAME_H_
#define _CONTENTNAME_H_

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

			const std::string &GetName() const
			{
				return _Name;
			}

			bool operator==(const ContentName &other) const
			{
				return other._Hash == _Hash;
			}
			
			DEFPROP_RO(public, unsigned int, Hash);
			DEFPROP_I_RO(public, const std::string &, Name, GetName);

		private:
			static unsigned int MakeHash(const std::string &s)
			{
				unsigned int hash = 0x1505;

				for (int i = s.size() - 1; i > 0; i--)
				{
					char c = (char)tolower((int)s[i]);

					if (c == '/')
						c = '\\';

					hash = hash * 0x21 + c;
				}

				return hash;
			}

			std::string _Name;
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
		struct ContentNameTraits
		{
			typedef std::unordered_map<ContentName, _Ty, ContenNameHasher> HashTable;
		};
	}
}

#endif
