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

#ifndef _STRINGRESOURCE_H_
#define _STRINGRESOURCE_H_

#include <YA3DE/Helpers.h>
#include <YA3DE/Content/Resource.h>

namespace YA3DE
{
	namespace Content
	{
		class StringResource : public Resource
		{
		public:
			StringResource(unsigned char *data, unsigned int size)
				: _Data(data), _Size(size)
			{
			}

			~StringResource()
			{
				if (_Data)
				{
					delete[] _Data;
					_Data = NULL;
				}
			}

			DEFPROP_RO(public, unsigned char *, Data);
			DEFPROP_RO(public, unsigned int, Size);
		};
		typedef std::shared_ptr<StringResource> StringResourcePtr;
	}
}

#endif
