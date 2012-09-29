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

#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>

#include <YA3DE/Helpers.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/Graphics/Texture2D.h>

using namespace YA3DE::Content;
using namespace YA3DE::Graphics;

namespace Graphics
{
	class World : Resource
	{
	public:
		void Update(double elapsed);
		void Render(double elapsed);
		
		DEFPROP_RO_R(public, std::vector<Texture2DPtr>, Textures);
	};
	typedef std::shared_ptr<World> WorldPtr;
}

#endif
