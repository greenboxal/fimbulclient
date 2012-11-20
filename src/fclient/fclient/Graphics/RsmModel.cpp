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

#include <fclient/Graphics/RsmModel.h>
#include <fclient/Graphics/VertexDeclarations.h>

#include <YA3DE/Logger.h>

#include <future>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace fclient;
using namespace YADE;

#define RSM_MAGIC "GRSM"

RsmMesh::RsmMesh(RsmModel *owner)
	: _Owner(owner), _BoundingBox(glm::vec3(999999), glm::vec3(-999999)), _HasGlobalMatrix(false), _HasLocalMatrix(false), _Elapsed(0)
{

}

bool RsmMesh::Load(RsmHeader &header, const YADE::FilePtr &stream)
{
#define IsCompatibleWith(major, minor) (header.Version.Major > major || (header.Version.Major == major && header.Version.Minor >= minor))
	char name[40], parentName[40];

	stream->Read(name, sizeof(name));
	stream->Read(parentName, sizeof(parentName));

	_Name = name;
	_ParentName = parentName;

	int textureCount;
	stream->Read(&textureCount, sizeof(int));

	_Textures.resize(textureCount);
	for (int i = 0; i < textureCount; i++)
	{
		int tex;

		stream->Read(&tex, sizeof(int));

		_Textures[i] = _Owner->Textures()[tex];
	}

	float transf[16];

	stream->Read(&transf[0], sizeof(float));
	stream->Read(&transf[1], sizeof(float));
	stream->Read(&transf[2], sizeof(float));
	transf[3] = 0;

	stream->Read(&transf[4], sizeof(float));
	stream->Read(&transf[5], sizeof(float));
	stream->Read(&transf[6], sizeof(float));
	transf[7] = 0;

	stream->Read(&transf[8], sizeof(float));
	stream->Read(&transf[9], sizeof(float));
	stream->Read(&transf[10], sizeof(float));
	transf[11] = 0;

	transf[12] = 0;
	transf[13] = 0;
	transf[14] = 0;
	transf[15] = 1;

	memcpy(&_ParentTransformation, transf, sizeof(glm::mat4));
	
	stream->Read(&_ParentPosition, sizeof(glm::vec3));
	stream->Read(&_Position, sizeof(glm::vec3));

	stream->Read(&_RotationAngle, sizeof(float));
	stream->Read(&_RotationAxis, sizeof(glm::vec3));

	stream->Read(&_Scale, sizeof(glm::vec3));

	int vertexCount;
	stream->Read(&vertexCount, sizeof(int));

	_TmpVertex.resize(vertexCount);
	stream->Read(&_TmpVertex[0], sizeof(glm::vec3) * vertexCount);

	std::vector<glm::vec2> texcoords;

	int texcoordCount;
	stream->Read(&texcoordCount, sizeof(int));
	
	texcoords.resize(texcoordCount);
	for (int i = 0; i < texcoordCount; i++)
	{
		if (IsCompatibleWith(1, 2))
			stream->Seek(1, 4);

		stream->Read(&texcoords[i], sizeof(glm::vec2));
	}

	int triangleCount;
	stream->Read(&triangleCount, sizeof(int));

	VertexPositionTextureNormal *vertices = new VertexPositionTextureNormal[triangleCount * 3];
	std::vector<int> *indices = new std::vector<int>[_Textures.size()];

	for (int i = 0; i < triangleCount; i++)
	{
		int start = i * 3;
		RsmFace face;

		stream->Read(&face, sizeof(RsmFace));

		glm::vec3 v1 = _TmpVertex[face.Vertices[0]];
		glm::vec3 v2 = _TmpVertex[face.Vertices[1]];
		glm::vec3 v3 = _TmpVertex[face.Vertices[2]];

		glm::vec2 t1 = texcoords[face.TexCoords[0]];
		glm::vec2 t2 = texcoords[face.TexCoords[1]];
		glm::vec2 t3 = texcoords[face.TexCoords[2]];

		glm::vec3 normal = glm::normalize(glm::cross(v1 - v3, v2 - v3));

		vertices[start + 0] = VertexPositionTextureNormal(v1, normal, t1);
		vertices[start + 1] = VertexPositionTextureNormal(v2, normal, t2);
		vertices[start + 2] = VertexPositionTextureNormal(v3, normal, t3);

		indices[face.TextureID].push_back(start + 0);
		indices[face.TextureID].push_back(start + 1);
		indices[face.TextureID].push_back(start + 2);
	}

	int rotFrameCount;
	stream->Read(&rotFrameCount, sizeof(int));

	_RotationFrames.resize(rotFrameCount);
	for (int i = 0; i < rotFrameCount; i++)
	{
		stream->Read(&_RotationFrames[i].first, sizeof(int));
		stream->Read(&_RotationFrames[i].second, sizeof(glm::quat));
	}

	_Vertices = std::make_shared<VertexBuffer>(VertexPositionTextureNormal::Declaration);
	_Vertices->SetData(vertices, triangleCount * 3, GL_STATIC_DRAW);

	_Indices.resize(_Textures.size());
	for (unsigned int i = 0; i < _Textures.size(); i++)
	{
		_Indices[i] = std::make_shared<IndexBuffer>(GL_UNSIGNED_INT);
		_Indices[i]->SetData(&indices[i][0], indices[i].size(), GL_STATIC_DRAW);
	}

	delete[] vertices;
	delete[] indices;
	
	return true;
#undef IsCompatibleWith
}

void RsmMesh::UpdateBoundingBox()
{
	if (_Parent != NULL)
		_BoundingBox = AABBox(glm::vec3(0.f), glm::vec3(0.f));

	for (unsigned int i = 0; i < _TmpVertex.size(); i++)
	{
		for (int n = 0; n < 3; n++)
		{
			glm::vec3 v = (_ParentTransformation * glm::vec4(_TmpVertex[i], 1)).xyz;

			if (_Parent != NULL || _Children.size() > 0)
				v += _Position + _ParentPosition;

			_BoundingBox += v;
		}
	}

	for (unsigned int i = 0; i < _Children.size(); i++)
	{
		//_Children[i]->UpdateBoundingBox();
		//_BoundingBox += _Children[i]->BoundingBox();
	}

	_BoundingBox.CalculateRangeAndOffset();
}

void RsmMesh::UpdateRealBoundingBox(AABBox &aabb, const glm::mat4 &ptm)
{
	UpdateGlobalMatrix(0);
	UpdateLocalMatrix();

	glm::mat4 myMat = ptm * _GlobalMatrix;
	glm::mat4 lMat = myMat * _LocalMatrix;
	
	for (unsigned int i = 0; i < _TmpVertex.size(); i++)
		aabb += (lMat * glm::vec4(_TmpVertex[i], 1)).xyz;
	
	for (unsigned int i = 0; i < _Children.size(); i++)
		_Children[i]->UpdateRealBoundingBox(aabb, myMat);

	_TmpVertex.clear();
}

void RsmMesh::UpdateGlobalMatrix(double elapsed)
{
	if (_HasGlobalMatrix)
		return;

	_GlobalMatrix = glm::mat4();

	if (_Parent == NULL)
	{
		if (_Children.size() == 0)
		{
			_GlobalMatrix = glm::translate(_GlobalMatrix, glm::vec3(0, -_Owner->MainMesh()->_BoundingBox.Max().y + _Owner->MainMesh()->_BoundingBox.Range().y, 0));
		}
		else
		{
			_GlobalMatrix = glm::translate(_GlobalMatrix, glm::vec3(-_Owner->MainMesh()->_BoundingBox.Range().x, -_Owner->MainMesh()->_BoundingBox.Max().y, -_Owner->MainMesh()->_BoundingBox.Range().z));
		}
	}
	else
	{
		_GlobalMatrix = glm::translate(_GlobalMatrix, _Position);
	}

	if (_RotationFrames.size() == 0)
	{
		if (_RotationAngle != -0.f)
			_GlobalMatrix = glm::rotate(_GlobalMatrix, glm::degrees(_RotationAngle), glm::vec3(+_RotationAxis.x, +_RotationAxis.y, +_RotationAxis.z));
	}
	else
	{
		if (elapsed > 0)
		{
			int current = 0;

			for (unsigned int i = 0; i < _RotationFrames.size(); i++)
            {
                if (_RotationFrames[i].first > _Elapsed)
                {
                    current = i - 1;
                    break;
                }
            }

			if (current < 0)
				current = 0;

			unsigned int next = current + 1;
			if (next >= _RotationFrames.size())
				next = 0;

			double interval = ((double)(_Elapsed - _RotationFrames[current].first) / ((double)(_RotationFrames[next].first - _RotationFrames[current].first)));
			glm::quat q = glm::mix(_RotationFrames[current].second, _RotationFrames[next].second, (float)interval);

			_GlobalMatrix *= glm::mat4_cast(glm::normalize(q));

			_Elapsed += elapsed;
			while (_Elapsed > _RotationFrames[_RotationFrames.size() - 1].first)
				_Elapsed -= _RotationFrames[_RotationFrames.size() - 1].first;
		}
		else
		{
			_GlobalMatrix *= glm::mat4_cast(glm::normalize(_RotationFrames[0].second));
		}
	}

	_GlobalMatrix = glm::scale(_GlobalMatrix, _Scale);
	
	if (_RotationFrames.size() == 0)
		_HasGlobalMatrix = true;
}
		
void RsmMesh::UpdateLocalMatrix()
{
	if (_HasLocalMatrix)
		return;

	_LocalMatrix = glm::mat4();

	if (_Parent == NULL && _Children.size() == 0)
		_LocalMatrix = glm::translate(_LocalMatrix, -_Owner->MainMesh()->_BoundingBox.Range());

	if (_Parent != NULL || _Children.size() > 0)
		_LocalMatrix = glm::translate(_LocalMatrix, _ParentPosition);

	_LocalMatrix *= _ParentTransformation;
	_HasLocalMatrix = true;
}

void RsmMesh::Update(double elapsed)
{
	if (!_Owner->Loaded())
		return;

	UpdateGlobalMatrix(elapsed);
	UpdateLocalMatrix();

	for (unsigned int i = 0; i < _Children.size(); i++)
		_Children[i]->Update(elapsed);
}

void RsmMesh::Render(ShaderProgramPtr &shader, Camera &camera, const glm::mat4 &model, double elapsed)
{
	if (!_Owner->Loaded())
		return;

	glm::mat4 world = model * _GlobalMatrix;

	shader->SetUniform("InTexture", 0);
	shader->SetUniform("WorldViewProjection", camera.GetProjection() * camera.GetView() * world * _LocalMatrix);
	_Vertices->Bind();
	for (unsigned int i = 0; i < _Indices.size(); i++)
	{
		_Textures[i]->Bind(0);
		_Vertices->Render(GL_TRIANGLES, _Indices[i], _Indices[i]->Count());
	}

	for (unsigned int i = 0; i < _Children.size(); i++)
		_Children[i]->Render(shader, camera, model, elapsed);
}

RsmModel::RsmModel()
	: _BoundingBox(glm::vec3(999999), glm::vec3(-999999)), _Loaded(false)
{

}

bool RsmModel::Load(YADE::FilePtr stream)
{
#define IsCompatibleWith(major, minor) (header.Version.Major > major || (header.Version.Major == major && header.Version.Minor >= minor))
	RsmHeader header;
	ContentManager *cm = ContentManager::Instance();

	stream->Read(&header, sizeof(RsmHeader));

	if (memcmp(header.Magic, RSM_MAGIC, sizeof(header.Magic)))
		return false;
	
	stream->Read(&_AnimationLength, sizeof(int));
	stream->Read(&_ShadeType, sizeof(int));
	
	if (IsCompatibleWith(1, 4))
		_Alpha = stream->ReadByte();
	else
		_Alpha = 255;

	stream->Seek(1, 16);

	int textureCount;
	stream->Read(&textureCount, sizeof(int));

	_Textures.resize(textureCount);
	for (int i = 0; i < textureCount; i++)
	{
		char name[40];
		stream->Read(name, sizeof(name));
		std::string namecopy = name;

		Texture2DPtr tex = cm->Load<Texture2D>(std::string("data/texture/") + name, true);

		tex->SetMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		tex->SetSTWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		_Textures[i] = tex;
	}

	char mainMeshName[40];
	stream->Read(mainMeshName, sizeof(mainMeshName));

	int meshCount;
	stream->Read(&meshCount, sizeof(int));

	for (int i = 0; i < meshCount; i++)
	{
		RsmMeshPtr mesh = std::make_shared<RsmMesh>(this);

		if (!mesh->Load(header, stream))
			return false;

		_Meshes[mesh->Name()] = mesh;
	}

	MeshList::iterator it = _Meshes.find(mainMeshName);

	if (it == _Meshes.end())
		return false;

	_MainMesh = it->second;

	{
		_MainMesh->Parent(nullptr);

		MeshList::iterator it;
		for (it = _Meshes.begin(); it != _Meshes.end(); it++)
		{
			if (it->second == _MainMesh)
				continue;

			it->second->Parent(_Meshes[it->second->ParentName()]);
			it->second->Parent()->Children().push_back(it->second);
		}
	}

	UpdateBoundingBox();
	
	_Loaded = true;

	return true;
#undef IsCompatibleWith
}

void RsmModel::UpdateBoundingBox()
{
	_MainMesh->UpdateBoundingBox();
	
	glm::mat4 mat = glm::scale(glm::mat4(), glm::vec3(1.f, -1.f, 1.f));
	_MainMesh->UpdateRealBoundingBox(_BoundingBox, mat);
	_BoundingBox.CalculateRangeAndOffset();
}

CONTENT_LOADER(RsmModel)
{
	auto namecopy = name;
	RsmModelPtr ptr = std::make_shared<RsmModel>();

	auto main = [=]()
	{
		FilePtr stream = FileManager::Instance()->OpenFile(namecopy);

		if (!stream)
		{
			LOG("Failed loading '%s': file not found", namecopy.c_str());
			return;
		}

		if (!ptr->Load(stream))
		{
			LOG("Failed loading '%s'", namecopy.c_str());
			return;
		}

		LOG("Loaded '%s'", namecopy.c_str());
	};

	if (async)
		ContentManager::Instance()->Dispatcher().EnqueueAsync(main);
	else
		main();

	ContentManager::CacheObject(name, ptr);

	return ptr;
}
