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

#include <YA3DE/Math.h>
#include <YA3DE/Graphics/VertexDeclaration.h>

namespace YA3DE
{
	namespace Graphics
	{
		struct VertexPositionTexture
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;

			VertexPositionTexture()
			{

			}

			VertexPositionTexture(const glm::vec3 &position, const glm::vec2 &texcoord)
			{
				Position = position;
				TexCoord = texcoord;
			}

			static VertexDeclaration VertexDeclaration;
		};
	}
}

