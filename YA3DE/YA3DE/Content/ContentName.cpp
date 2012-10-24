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

#include <stdafx.h>
#include <YA3DE/Content/ContentName.h>

using namespace YA3DE::Content;

unsigned int ContentName::MakeHash(const std::string &s)
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
