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

#ifndef _RSMMODEL_H_
#define _RSMMODEL_H_

#include <map>
#include <vector>
#include <string>

#include <YA3DE/Helpers.h>
#include <YA3DE/FileSystem/FileManager.h>
#include <YA3DE/Content/ContentManager.h>

#include <YA3DE/Math.h>
#include <YA3DE/Math/AABBox.h>
#include <YA3DE/Scene/Camera.h>

#include <YA3DE/Graphics/ShaderProgram.h>
#include <YA3DE/Graphics/VertexDeclaration.h>
#include <YA3DE/Graphics/VertexBuffer.h>
#include <YA3DE/Graphics/IndexBuffer.h>
#include <YA3DE/Graphics/Texture2D.h>

#include <glm/gtc/quaternion.hpp>

namespace ROGraphics
{
	using namespace YA3DE::Math;
	using namespace YA3DE::Scene;
	using namespace YA3DE::Content;
	using namespace YA3DE::Graphics;

#include <YA3DE/Pack1.h>
	struct RsmHeader
	{
		char Magic[4];
		union
		{
			struct
			{
				unsigned char Major;
				unsigned char Minor;
			};
			unsigned short Version;
		} Version;
	} STRUCT_PACKED;

	struct RsmFace
	{
		unsigned short Vertices[3];
		unsigned short TexCoords[3];
		unsigned short TextureID;
		unsigned short Unknown;
		unsigned int Dummy[2];
	} STRUCT_PACKED;
#include <YA3DE/PackEnd.h>

	class RsmModel;
	class RsmMesh : public std::enable_shared_from_this<RsmMesh>
	{
	public:
		typedef std::vector<std::pair<int, glm::quat> > RotationList;

		RsmMesh(RsmModel *owner);
		bool Load(RsmHeader &header, const YA3DE::FileSystem::FilePtr &stream);

		void UpdateBoundingBox();
		void UpdateRealBoundingBox(AABBox &aabb, const glm::mat4 &ptm);

		void Update(double elapsed);
		void Render(ShaderProgramPtr &shader, Camera &camera, const glm::mat4 &model, double elapsed);

		DEFPROP_RO_R(public, std::string, Name);
		DEFPROP_RO_R(public, std::string, ParentName);
		DEFPROP_RO_R(public, std::vector<Texture2DPtr>, Textures);
		DEFPROP_RO_R(public, glm::mat4, ParentTransformation);
		DEFPROP_RO_R(public, glm::vec3, ParentPosition);
		DEFPROP_RO_R(public, glm::vec3, Position);
		DEFPROP_RO_R(public, VertexBufferPtr, Vertices);
		DEFPROP_RO_R(public, std::vector<IndexBufferPtr>, Indices);
		DEFPROP_RO_R(public, RotationList, RotationFrames);
		DEFPROP_RO_R(public, float, RotationAngle);
		DEFPROP_RO_R(public, glm::vec3, RotationAxis);
		DEFPROP_RO_R(public, glm::vec3, Scale);
		DEFPROP_RO_R(public, AABBox, BoundingBox);
		DEFPROP_RW(public, std::shared_ptr<RsmMesh>, Parent);
		DEFPROP_RO_R(public, std::vector<std::shared_ptr<RsmMesh>>, Children);
		DEFPROP_RO(public, RsmModel *, Owner);
	
	private:
		std::vector<glm::vec3> _TmpVertex;

		double _Elapsed;

		void UpdateGlobalMatrix(double elapsed);
		glm::mat4 _GlobalMatrix;
		bool _HasGlobalMatrix;
		
		void UpdateLocalMatrix();
		glm::mat4 _LocalMatrix;
		bool _HasLocalMatrix;
	};
	typedef std::shared_ptr<RsmMesh> RsmMeshPtr;
	
	class RsmModel : public Resource
	{
	public:
		typedef std::map<std::string, RsmMeshPtr> MeshList;

		RsmModel();

		bool Load(YA3DE::FileSystem::FilePtr stream);
		void UpdateBoundingBox();

		DEFPROP_RO(public, int, AnimationLength);
		DEFPROP_RO(public, int, ShadeType);
		DEFPROP_RO(public, unsigned char, Alpha);
		DEFPROP_RO_R(public, std::vector<Texture2DPtr>, Textures);
		DEFPROP_RO(public, RsmMeshPtr, MainMesh);
		DEFPROP_RO_R(public, MeshList, Meshes);
		DEFPROP_RO_R(public, AABBox, BoundingBox);

		DEFPROP_RO(public, bool, Loaded);
	};
	typedef std::shared_ptr<RsmModel> RsmModelPtr;
}

#endif
