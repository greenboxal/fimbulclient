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

#ifndef _WORLDOBJECTS_H_
#define _WORLDOBJECTS_H_

#include "RsmModel.h"
#include "World.h"

#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>
#include <YA3DE/FileSystem/IFile.h>
#include <YA3DE/Graphics/ShaderProgram.h>

namespace ROGraphics
{
	class WorldModel : public SceneNode
	{
	public:
		WorldModel(World *owner);

		bool Load(YA3DE::FileSystem::FilePtr stream, int majorVersion, int minorVersion);
		
		virtual void Update(Camera *camera, double elapsed);
		virtual void Render(Camera *camera, double elapsed);

		DEFPROP_RO_R(public, std::string, Name);
		DEFPROP_RO(public, int, AnimationType);
		DEFPROP_RO(public, float, AnimationSpeed);
		DEFPROP_RO(public, int, BlockType);
		DEFPROP_RO_R(public, std::string, ModelName);
		DEFPROP_RO_R(public, std::string, MeshName);
		DEFPROP_RW_R(public, glm::vec3, Position);
		DEFPROP_RW_R(public, glm::vec3, Rotation);
		DEFPROP_RW_R(public, glm::vec3, Scale);
		DEFPROP_RO(public, RsmModelPtr, Model);
		DEFPROP_RO(public, RsmMeshPtr, Mesh);
		DEFPROP_RO_R(public, glm::mat4, WorldMatrix);

	private:
		ShaderProgramPtr _Shader;
		World *_Owner;
	};
}

#endif
