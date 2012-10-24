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
#include <YA3DE/Logger.h>
#include <YA3DE/Config.h>
#include <rapidxml/rapidxml_utils.hpp>

using namespace YA3DE;
using namespace rapidxml;

void Config::Read()
{
	LOG("Reading configuration");

	file<> f(_Name.c_str());
	char *xml = new char[f.size()];
	memcpy(xml, f.data(), f.size());

	_Root.parse<rapidxml::parse_delete_on_destruct>(xml);
}
