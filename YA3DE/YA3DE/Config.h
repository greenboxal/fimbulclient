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

#include <rapidxml/rapidxml.hpp>

#include <YA3DE/Helpers.h>

namespace YA3DE
{
	class Config
	{
	public:
		Config(std::string filename)
			: _Name(filename)
		{
		}

		void Read();

		DEFPROP_RO(public, std::string, Name);
		DEFPROP_RO_R(public, rapidxml::xml_document<>, Root);
	};
}
