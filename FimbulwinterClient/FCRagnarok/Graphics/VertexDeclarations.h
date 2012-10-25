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

#pragma once

#include <YA3DE/Helpers.h>
#include <YA3DE/Math.h>
#include <YA3DE/Graphics/VertexDeclaration.h>

namespace FCRagnarok
{
	namespace Graphics
	{
		#include <YA3DE/Pack1.h>
		struct VertexPositionTextureNormal
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TextureCoord;

			VertexPositionTextureNormal()
				: Position(0), Normal(0), TextureCoord(0)
			{

			}

			VertexPositionTextureNormal(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texcoord)
				: Position(position), Normal(normal), TextureCoord(texcoord)
			{

			}

			static YA3DE::Graphics::VertexDeclaration Declaration;
		};

		struct VertexPositionTextureColorNormalLightmap
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TextureCoord;
			glm::vec2 LightmapCoord;
			glm::vec3 Color;

			VertexPositionTextureColorNormalLightmap()
				: Position(0), Normal(0), TextureCoord(0), LightmapCoord(0)
			{

			}

			VertexPositionTextureColorNormalLightmap(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texcoord, const glm::vec2 &lmapcoord, const glm::vec3 &color)
				: Position(position), Normal(normal), TextureCoord(texcoord), LightmapCoord(lmapcoord), Color(color)
			{

			}

			static YA3DE::Graphics::VertexDeclaration Declaration;
		};
		#include <YA3DE/PackEnd.h>
	}
}

