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

#include "WorldObjects.h"
#include "World.h"

using namespace ROGraphics;

WorldModel::WorldModel(World *owner)
	: _Owner(owner)
{

}

bool WorldModel::Load(YA3DE::FileSystem::FilePtr stream, int majorVersion, int minorVersion)
{
#define IsCompatibleWith(major, minor) (majorVersion > major || (majorVersion == major && minorVersion >= minor))
	if (IsCompatibleWith(1, 3))
	{
		char name[40];

		stream->Read(name, sizeof(name));
		stream->Read(&_AnimationType, sizeof(int));
		stream->Read(&_AnimationSpeed, sizeof(float));

		if (_AnimationSpeed < 0.f || _AnimationSpeed >= 100.f)
			_AnimationSpeed = 1.f;

		stream->Read(&_BlockType, sizeof(int));

		_Name = name;
	}

	char modelName[80];
	char meshName[80];

	stream->Read(modelName, sizeof(modelName));
	stream->Read(meshName, sizeof(meshName));

	stream->Read(&_Position, sizeof(glm::vec3));
	stream->Read(&_Rotation, sizeof(glm::vec3));
	stream->Read(&_Scale, sizeof(glm::vec3));

	_ModelName = modelName;
	_MeshName = meshName;

	return true;
#undef IsCompatibleWith
}

bool WorldModel::DoPostLoad()
{
	_Model = ContentManager::Instance()->Load<RsmModel>(std::string("data/model/" + _ModelName));
	
	if (_Model == NULL)
		return false;
	
	RsmModel::MeshList::iterator it = _Model->Meshes.find(_MeshName);

	if (it == _Model->Meshes.end())
		_Mesh = _Model->MainMesh;
	else
		_Mesh = it->second;

    float offsetX = _Owner->GroundZoom * _Owner->GroundWidth / 2;
    float offsetZ = _Owner->GroundZoom * _Owner->GroundHeight / 2;

	_WorldMatrix = glm::translate(_WorldMatrix, glm::vec3(-offsetX + _Position.x * 5, _Position.y, -offsetZ + _Position.z * 5));
	_WorldMatrix = glm::rotate(_WorldMatrix, -_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
	_WorldMatrix = glm::rotate(_WorldMatrix, -_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
	_WorldMatrix = glm::rotate(_WorldMatrix, _Rotation.y, glm::vec3(0.f, 1.f, 0.f));
	_WorldMatrix = glm::scale(_WorldMatrix, _Scale);

	return true;
}
		
void WorldModel::Update(double elapsed)
{
	_Mesh->Update(elapsed);
}

void WorldModel::Render(CommonShaderProgramPtr &shader, Camera &camera, double elapsed)
{
	shader->SetAlpha(_Model->Alpha / 255.f);
	_Mesh->Render(shader, camera, _WorldMatrix, elapsed);
}
