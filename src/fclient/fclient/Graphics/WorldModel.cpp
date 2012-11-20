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

#include <fclient/Graphics/WorldModel.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace fclient;
using namespace YADE;

WorldModel::WorldModel(World *owner)
	: SceneNode(owner->SceneRoot()), _Owner(owner)
{

}

bool WorldModel::Load(YADE::FilePtr stream, int majorVersion, int minorVersion)
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
	
	_Shader = ContentManager::Instance()->Load<ShaderProgram>("data/shaders/model.glsl");
	_Model = ContentManager::Instance()->Load<RsmModel>(std::string("data/model/" + _ModelName), true);
	
	if (_Model == nullptr)
		return false;

	_Position.x = (_Position.x / 5) + _Owner->GroundWidth();
	_Position.z = (_Position.z / 5) + _Owner->GroundHeight();

	_WorldMatrix = glm::translate(_WorldMatrix, glm::vec3(0, 0, _Owner->GroundHeight() * _Owner->GroundZoom()));
	_WorldMatrix = glm::scale(_WorldMatrix, glm::vec3(1, 1, -1));
	_WorldMatrix = glm::translate(_WorldMatrix, glm::vec3(_Position.x * 5, -_Position.y, _Position.z * 5));
	_WorldMatrix = glm::rotate(_WorldMatrix, -_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
	_WorldMatrix = glm::rotate(_WorldMatrix, -_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
	_WorldMatrix = glm::rotate(_WorldMatrix, _Rotation.y, glm::vec3(0.f, 1.f, 0.f));
	_WorldMatrix = glm::scale(_WorldMatrix, glm::vec3(_Scale.x, -_Scale.y, _Scale.z));
	_WorldMatrix = glm::translate(_WorldMatrix, glm::vec3(-_Model->BoundingBox().Range().x, _Model->BoundingBox().Min().y, -_Model->BoundingBox().Range().z));
	
	return true;
#undef IsCompatibleWith
}

void WorldModel::Update(Camera *camera, double elapsed)
{
	if (_Model == nullptr)
		return;

	if (_Mesh == nullptr)
	{
		RsmModel::MeshList::iterator it = _Model->Meshes().find(_MeshName);

		if (it == _Model->Meshes().end())
			_Mesh = _Model->MainMesh();
		else
			_Mesh = it->second;

		if (_Mesh == nullptr)
			return;
	}

	_Mesh->Update(elapsed);
}

void WorldModel::Render(Camera *camera, double elapsed)
{
	if (_Mesh == nullptr)
		return;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	_Shader->Begin();
	_Shader->SetUniform("Alpha", _Model->Alpha() / 255.f);
	_Mesh->Render(_Shader, *camera, _WorldMatrix, elapsed);
	_Shader->End();
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}
