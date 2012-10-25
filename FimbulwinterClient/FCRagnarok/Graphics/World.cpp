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

/*
	TODO: Add normals calculation
*/

#include <stdafx.h>
#include <FCRagnarok/Graphics/World.h>
#include <FCRagnarok/Graphics/WorldModel.h>
#include <FCRagnarok/Graphics/VertexDeclarations.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <YA3DE/Math.h>
#include <YA3DE/Logger.h>
#include <YA3DE/Content/StringResource.h>

using namespace FCRagnarok::Graphics;
using namespace YA3DE::FileSystem;

#define GND_MAGIC "GRGN"
#define GAT_MAGIC "GRAT"
#define RSW_MAGIC "GRSW"

typedef struct SmoothNode 
{
    unsigned int Index;
    bool Averaged;
    struct SmoothNode *Next;
} SmoothNode;


World::World()
{

}

bool World::LoadFromRsw(FilePtr rswFile)
{
	_GroundShader = ContentManager::Instance()->Load<ShaderProgram>("data/shaders/ground.glsl");
	if (!_GroundShader)
		return false;
		
	_CommonShader = ContentManager::Instance()->Load<ShaderProgram>("data/shaders/model.glsl");
	if (!_CommonShader)
		return false;

	if (!LoadWorld(rswFile))
		return false;

	return true;
}

void World::SetupGroundLightmap(std::vector<LightmapCell> &lmaps)
{
	int w = (int)std::floor(std::sqrt((float)lmaps.size()));
    int h = (int)std::ceil((float)lmaps.size() / (float)w);
	float *lmapData = new float[w * h * 8 * 8 * 4];

	int x = 0, y = 0;
	for (unsigned int i = 0; i < lmaps.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int offset = (y * w * 8 * 8 + j * w * 8 + x * 8) * 4;

			for (int k = 0; k < 8; k++)
			{
#define POSTERIZE(color, levels) (((int)((float)(color) / levels)) * levels)
				lmapData[offset + k * 4 + 0] = POSTERIZE(lmaps[i].Color[j * 8 + k].R, 16.f) / 255.f;
				lmapData[offset + k * 4 + 1] = POSTERIZE(lmaps[i].Color[j * 8 + k].G, 16.f) / 255.f;
				lmapData[offset + k * 4 + 2] = POSTERIZE(lmaps[i].Color[j * 8 + k].B, 16.f) / 255.f;
				lmapData[offset + k * 4 + 3] = lmaps[i].Brightness[j * 8 + k] / 255.f;
#undef POSTERIZE
			}
		}

		if (++y >= h)
		{
			y = 0;
			x++;
		}
	}
	
	_Lightmap = std::make_shared<Texture2D>(w * 8, h * 8, GL_RGBA, GL_RGBA, GL_FLOAT);
	_Lightmap->SetData(lmapData);
	_Lightmap->SetMinMagFilter(GL_LINEAR, GL_LINEAR);
	_Lightmap->SetSTWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	delete[] lmapData;
}

void World::SetupGroundVertices(std::vector<LightmapCell> &lmaps, std::vector<Surface> &surfaces, std::vector<GroundCell> &cells)
{
	int objectCount = 0;
	struct NormalInfo
	{
		glm::vec3 Flat;
		glm::vec3 Smoothed[4];
	} *normals = new NormalInfo[_GroundWidth * _GroundHeight];

	// Count object count & calculate normals
	for (int x = 0; x < _GroundWidth; x++)
	{
		for (int y = 0; y < _GroundHeight; y++)
		{
			int index = y * _GroundWidth + x;
			GroundCell &cell = cells[index];

			if (cell.TopSurfaceID != -1)
				objectCount++;

			if (cell.FrontSurfaceID != -1)
				objectCount++;

			if (cell.RightSurfaceID != -1)
				objectCount++;

			glm::vec3 b1 = glm::vec3(_GroundZoom, -cell.Height[0], -_GroundZoom) - glm::vec3(0, -cell.Height[3], 0);
			glm::vec3 b2 = glm::vec3(0, -cell.Height[2], -_GroundZoom) - glm::vec3(0, -cell.Height[3], 0);

			normals[index].Flat = glm::normalize(glm::cross(b1, b2));
		}
	}

	// Smooth normals
	for (int x = 0; x < _GroundWidth; x++)
	{
		for (int y = 0; y < _GroundHeight; y++)
		{
#define CELLN(x, y) normals[(y) * _GroundWidth + (x)]
#define SMOOTH(i, x, y) info.Smoothed[i] += CELLN(x, y).Flat; num++
			NormalInfo &info = CELLN(x, y);
			float num = 0;

			info.Smoothed[0] = info.Flat;
			info.Smoothed[1] = info.Flat;
			info.Smoothed[2] = info.Flat;
			info.Smoothed[3] = info.Flat;

			if (x > 0)
			{
				SMOOTH(0, x - 1, y);
				SMOOTH(1, x - 1, y);
				
				if (y > 0)
					SMOOTH(0, x - 1, y - 1);

				if (y < _GroundHeight - 1)
					SMOOTH(1, x - 1, y + 1);
			}

			if (y > 0)
			{
				SMOOTH(0, x, y - 1);
				SMOOTH(2, x, y - 1);

				if (x > 0)
					SMOOTH(0, x - 1, y - 1);
				
				if (x < _GroundWidth - 1)
					SMOOTH(2, x + 1, y - 1);
			}

			if (x < _GroundWidth - 1)
			{
				SMOOTH(2, x + 1, y);
				SMOOTH(3, x + 1, y);

				if (y < _GroundHeight - 1)
					SMOOTH(3, x + 1, y + 1);
			}

			if (y < _GroundHeight - 1)
			{
				SMOOTH(1, x, y + 1);
				SMOOTH(3, x, y + 1);

				if (x < _GroundWidth - 1)
					SMOOTH(3, x + 1, y + 1);
			}

			if (num == 0)
				num++;

			info.Smoothed[0] = glm::normalize(info.Smoothed[0] / num);
			info.Smoothed[1] = glm::normalize(info.Smoothed[1] / num);
			info.Smoothed[2] = glm::normalize(info.Smoothed[2] / num);
			info.Smoothed[3] = glm::normalize(info.Smoothed[3] / num);
#undef SMOOTH
#undef CELLN
		}
	}

	VertexPositionTextureColorNormalLightmap *vertices = new VertexPositionTextureColorNormalLightmap[objectCount * 4];
	std::vector<int> *indices = new std::vector<int>[_GroundTextures.size()];
	int currentSurface = 0;

	auto AddSurface = [&](int x, int y, int surfaceID, int type) 
	{
		int idx = currentSurface * 4;
		int cellIdx = y * _GroundWidth + x;

		GroundCell &cell = cells[cellIdx];
		Surface &surface = surfaces[surfaceID];

		glm::vec3 position[4];
		glm::vec3 normal[4];

		switch (type)
		{
			case 0:
				{
					float x0 = x * _GroundZoom;
					float x1 = (x + 1) * _GroundZoom;

					float z0 = (_GroundHeight - y) * _GroundZoom;
					float z1 = (_GroundHeight - y - 1) * _GroundZoom;

					position[0] = glm::vec3(x0, -cell.Height[0], z0);
					position[1] = glm::vec3(x1, -cell.Height[1], z0);
					position[2] = glm::vec3(x0, -cell.Height[2], z1);
					position[3] = glm::vec3(x1, -cell.Height[3], z1);

					normal[0] = normals[cellIdx].Smoothed[0];
					normal[1] = normals[cellIdx].Smoothed[1];
					normal[2] = normals[cellIdx].Smoothed[2];
					normal[3] = normals[cellIdx].Smoothed[3];
				}
				break;
			case 1:
				{
					GroundCell &cell2 = cells[(y + 1) * _GroundWidth + x];

					float x0 = x * _GroundZoom;
					float x1 = (x + 1) * _GroundZoom;

					float z0 = (_GroundHeight - y - 1) * _GroundZoom;

					position[0] = glm::vec3(x0, -cell.Height[2], z0);
					position[1] = glm::vec3(x1, -cell.Height[3], z0);
					position[2] = glm::vec3(x0, -cell2.Height[0], z0);
					position[3] = glm::vec3(x1, -cell2.Height[1], z0);

					normal[0] = glm::vec3(0, 0, cell2.Height[0] > cell.Height[3] ? -1 : 1);
					normal[1] = normal[0];
					normal[2] = normal[0];
					normal[3] = normal[0];
				}
				break;
			case 2:
				{
					GroundCell &cell2 = cells[y * _GroundWidth + x + 1];

					float x0 = (x + 1) * _GroundZoom;
						
					float z0 = (_GroundHeight - y) * _GroundZoom;
					float z1 = (_GroundHeight - y - 1) * _GroundZoom;

					position[0] = glm::vec3(x0, -cell.Height[3], z1);
					position[1] = glm::vec3(x0, -cell.Height[1], z0);
					position[2] = glm::vec3(x0, -cell2.Height[2], z1);
					position[3] = glm::vec3(x0, -cell2.Height[0], z0);

					normal[0] = glm::vec3(cell.Height[3] > cell2.Height[2] ? -1 : 1, 0, 0);
					normal[1] = normal[0];
					normal[2] = normal[0];
					normal[3] = normal[0];
				}
				break;
		}

		int lmW = (int)std::floor(std::sqrt((float)lmaps.size()));
		int lmH = (int)std::ceil((float)lmaps.size() / (float)lmW);
		int lmX = (int)std::floor((float)surface.LightmapID / (float)lmH);
		int lmY = surface.LightmapID % lmH;

		float lightmapU[2];
		float lightmapV[2];

		lightmapU[0] = (0.1f + lmX) / lmW;
		lightmapU[1] = (0.9f + lmX) / lmW;
		lightmapV[0] = (0.1f + lmY) / lmH;
		lightmapV[1] = (0.9f + lmY) / lmH;

		glm::vec3 colors[4];

		if (type == 0)
		{
#define FIND_COLOR(stor, x1, y1) \
			{ \
				int xx = x1; \
				int yy = y1; \
				if (xx >= 0 && xx < _GroundWidth && yy >= 0 && yy < _GroundHeight) \
				{ \
					int idx = yy * _GroundWidth + xx; \
					if (cells[idx].TopSurfaceID != -1) \
					{ \
						Surface &s = surfaces[cells[idx].TopSurfaceID]; \
						colors[stor] = glm::vec3(s.Color.R / 255.f, s.Color.G / 255.f, s.Color.B / 255.f); \
					} \
				} \
			}
		
			FIND_COLOR(0, x, y);
			FIND_COLOR(1, x + 1, y);
			FIND_COLOR(2, x, y + 1);
			FIND_COLOR(3, x + 1, y + 1);
#undef FIND_COLOR
		}
		else
		{
			colors[0] = glm::vec3(1.f);
			colors[1] = glm::vec3(1.f);
			colors[2] = glm::vec3(1.f);
			colors[3] = glm::vec3(1.f);
		}

		vertices[idx + 0] = VertexPositionTextureColorNormalLightmap(position[0], normal[0], glm::vec2(surface.U[0], surface.V[0]), glm::vec2(lightmapU[0], lightmapV[0]), colors[0]);
		vertices[idx + 1] = VertexPositionTextureColorNormalLightmap(position[1], normal[1], glm::vec2(surface.U[1], surface.V[1]), glm::vec2(lightmapU[1], lightmapV[0]), colors[1]);
		vertices[idx + 2] = VertexPositionTextureColorNormalLightmap(position[2], normal[2], glm::vec2(surface.U[2], surface.V[2]), glm::vec2(lightmapU[0], lightmapV[1]), colors[2]);
		vertices[idx + 3] = VertexPositionTextureColorNormalLightmap(position[3], normal[3], glm::vec2(surface.U[3], surface.V[3]), glm::vec2(lightmapU[1], lightmapV[1]), colors[3]);

		indices[surface.TextureID].push_back(idx + 0);
		indices[surface.TextureID].push_back(idx + 1);
		indices[surface.TextureID].push_back(idx + 2);
		indices[surface.TextureID].push_back(idx + 2);
		indices[surface.TextureID].push_back(idx + 1);
		indices[surface.TextureID].push_back(idx + 3);
	};

	for (int x = 0; x < _GroundWidth; x++)
	{
		for (int y = 0; y < _GroundHeight; y++)
		{
			GroundCell &cell = cells[y * _GroundWidth + x];

			if (cell.TopSurfaceID != -1)
			{
				int tid = surfaces[cell.TopSurfaceID].TextureID;

				AddSurface(x, y, cell.TopSurfaceID, 0);

				currentSurface++;
			}

			if (cell.FrontSurfaceID != -1)
			{
				int tid = surfaces[cell.FrontSurfaceID].TextureID;
				
				AddSurface(x, y, cell.FrontSurfaceID, 1);

				currentSurface++;
			}

			if (cell.RightSurfaceID != -1)
			{
				int tid = surfaces[cell.RightSurfaceID].TextureID;
				
				AddSurface(x, y, cell.RightSurfaceID, 2);

				currentSurface++;
			}
		}
	}

	_GroundVBuffer = std::make_shared<VertexBuffer>(VertexPositionTextureColorNormalLightmap::Declaration);
	_GroundVBuffer->SetData(vertices, objectCount * 4, GL_STATIC_DRAW);

	_GroundIBuffers.resize(_GroundTextures.size());
	for (unsigned int i = 0; i < _GroundTextures.size(); i++)
	{
		_GroundIBuffers[i] = std::make_shared<IndexBuffer>(GL_UNSIGNED_INT);
		_GroundIBuffers[i]->SetData(&indices[i][0], indices[i].size(), GL_STATIC_DRAW);
	}

	delete[] vertices;
	delete[] indices;
}

bool World::LoadGround(FilePtr stream)
{
	GndHeader header;
	ContentManager *cm = ContentManager::Instance();

	stream->Read(&header, sizeof(GndHeader));

	if (memcmp(header.Magic, GND_MAGIC, sizeof(header.Magic)))
		return false;

	if (!(header.Version.Major == 1 && header.Version.Minor == 7))
		return false;

	if (header.Width < 0 || header.Height < 0)
		return false;

	_GroundWidth = header.Width;
	_GroundHeight = header.Height;
	_GroundZoom = header.Zoom;

	if (header.TextureCount < 0 || header.TextureSize < 0)
		return false;

	char *name = new char[header.TextureSize];
	_GroundTextures.resize(header.TextureCount);
	for (int i = 0; i < header.TextureCount; i++)
	{
		stream->Read(name, header.TextureSize);
		_GroundTextures[i] = cm->Load<Texture2D>(std::string("data/texture/") + name, true);
		_GroundTextures[i]->SetMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		_GroundTextures[i]->SetSTWrap(GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
	}
	delete[] name;

	int lmapCount, lmapWidth, lmapHeight, lmapCells;
	stream->Read(&lmapCount, sizeof(int));
	stream->Read(&lmapWidth, sizeof(int));
	stream->Read(&lmapHeight, sizeof(int));
	stream->Read(&lmapCells, sizeof(int));

	if (lmapCount < 0 || lmapWidth != 8 || lmapHeight != 8 || lmapCells != 1)
		return false;

	std::vector<LightmapCell> lmaps;
	lmaps.resize(lmapCount);
	stream->Read(&lmaps[0], sizeof(LightmapCell) * lmapCount);

	int surfaceCount;
	stream->Read(&surfaceCount, sizeof(int));

	if (surfaceCount <= 0)
		return false;

	std::vector<Surface> surfaces;
	surfaces.resize(surfaceCount);
	stream->Read(&surfaces[0], sizeof(Surface) * surfaceCount);
	
	std::vector<GroundCell> cells;
	cells.resize(header.Width * header.Height);
	stream->Read(&cells[0], sizeof(GroundCell) * cells.size());
	
	SetupGroundLightmap(lmaps);
	SetupGroundVertices(lmaps, surfaces, cells);
	
	return true;
}

bool World::LoadAltitude(YA3DE::FileSystem::FilePtr stream)
{
	GatHeader header;
	ContentManager *cm = ContentManager::Instance();

	stream->Read(&header, sizeof(GatHeader));

	if (memcmp(header.Magic, GAT_MAGIC, sizeof(header.Magic)))
		return false;
	
	if (!(header.Version.Major == 1 && header.Version.Minor == 2))
		return false;

	if (header.Width < 0 || header.Height < 0)
		return false;

	_AltitudeWidth = header.Width;
	_AltitudeHeight = header.Height;

	_AltitudeCells.resize(header.Width * header.Height);
	stream->Read(&_AltitudeCells[0], sizeof(AltitudeCell) * _AltitudeCells.size());

	return true;
}

bool World::SetupWaterVertices()
{
	ContentManager *cm = ContentManager::Instance();
	VertexPositionTextureNormal vertexdata[4];
    short indexdata[4];

    glm::vec3 position[4];
    glm::vec2 tex[4];

    float x0 = 0;
    float x1 = _GroundWidth * _GroundZoom;

    float z0 = 0;
    float z1 = _GroundHeight * _GroundZoom;
            
    position[0] = glm::vec3(x0, -_Water.Level, z0);
    position[1] = glm::vec3(x1, -_Water.Level, z0);
    position[2] = glm::vec3(x0, -_Water.Level, z1);
    position[3] = glm::vec3(x1, -_Water.Level, z1);

    tex[0] = glm::vec2(0, 0);
    tex[1] = glm::vec2(_GroundWidth / 8, 0);
    tex[2] = glm::vec2(0, _GroundHeight / 8);
    tex[3] = glm::vec2(_GroundWidth / 8, _GroundHeight / 8);

    vertexdata[0] = VertexPositionTextureNormal(position[0], glm::vec3(1.0F), tex[0]);
    vertexdata[1] = VertexPositionTextureNormal(position[1], glm::vec3(1.0F), tex[1]);
    vertexdata[2] = VertexPositionTextureNormal(position[2], glm::vec3(1.0F), tex[2]);
    vertexdata[3] = VertexPositionTextureNormal(position[3], glm::vec3(1.0F), tex[3]);

    indexdata[0] = 0;
    indexdata[1] = 1;
    indexdata[2] = 2;
    indexdata[3] = 3;

	_WaterVBuffer = std::make_shared<VertexBuffer>(VertexPositionTextureNormal::Declaration);
	_WaterVBuffer->SetData(vertexdata, 4, GL_STATIC_DRAW);

	_WaterIBuffer = std::make_shared<IndexBuffer>(GL_UNSIGNED_SHORT);
	_WaterIBuffer->SetData(indexdata, 4, GL_STATIC_DRAW);

	for (int i = 0; i < 32; i++)
	{
		char name[8];

		sprintf(name, "%d%02d", _Water.Type, i);

		Texture2DPtr tex = cm->Load<Texture2D>(std::string("data/texture/ฟ๖ลอ/water") + name + ".jpg", true);

		if (tex == NULL)
			return false;

		tex->SetMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		tex->SetSTWrap(GL_REPEAT, GL_REPEAT);

		_WaterTextures.push_back(tex);
	}

	_WaterTextureIndex = 0;
	_WaterElapsed = 0;

	return true;
}

bool World::LoadWorld(YA3DE::FileSystem::FilePtr stream)
{
#define IsCompatibleWith(major, minor) (header.Version.Major > major || (header.Version.Major == major && header.Version.Minor >= minor))
	RswHeader header;
	ContentManager *cm = ContentManager::Instance();
	FileManager *fm = FileManager::Instance();
	
	stream->Read(&header, sizeof(RswHeader));

	if (memcmp(header.Magic, RSW_MAGIC, sizeof(header.Magic)))
		return false;

	char iniFileName[40];
	char gndFileName[40];
	char gatFileName[40];
	char scrFileName[40];

	stream->Read(iniFileName, sizeof(iniFileName));
	stream->Read(gndFileName, sizeof(gndFileName));

	if (IsCompatibleWith(1, 3))
	{
		stream->Read(gatFileName, sizeof(gatFileName));
	}
	else
	{
		memset(gatFileName, 0, sizeof(gatFileName));
	}
	
	stream->Read(scrFileName, sizeof(scrFileName));
	
	iniFileName[39] = 0;
	gndFileName[39] = 0;
	gatFileName[39] = 0;
	scrFileName[39] = 0;

	FilePtr gatFile = fm->OpenFile(std::string("data/") + gatFileName);
	FilePtr gndFile = fm->OpenFile(std::string("data/") + gndFileName);

	if (gatFile && !LoadAltitude(gatFile))
		return false;

	if (!gndFile || !LoadGround(gndFile))
		return false;

	if (IsCompatibleWith(1, 3))
	{
		stream->Read(&_Water.Level, sizeof(float));
	}
	else
	{
		_Water.Level = 0;
	}

	if (IsCompatibleWith(1, 8))
	{
		stream->Read(&_Water.Type, sizeof(int));
		stream->Read(&_Water.WaveHeight, sizeof(float));
		stream->Read(&_Water.WaveSpeed, sizeof(float));
		stream->Read(&_Water.WavePitch, sizeof(float));
	}
	else
	{
		_Water.Type = 0;
		_Water.WaveHeight = 1.f;
		_Water.WaveSpeed = 2.f;
		_Water.WavePitch = 50.f;
	}

	if (IsCompatibleWith(1, 9))
	{
		stream->Read(&_Water.AnimSpeed, sizeof(float));
	}
	else
	{
		_Water.AnimSpeed = 3;
	}
	
	if (IsCompatibleWith(1, 5))
	{
		stream->Read(&Light.Longitude, sizeof(int));
		stream->Read(&Light.Latitude, sizeof(int));
		stream->Read(&Light.Diffuse, sizeof(glm::vec3));
		stream->Read(&Light.Ambient, sizeof(glm::vec3));
	}
	else
	{
		Light.Longitude = 45;
		Light.Latitude = 45;
		Light.Diffuse = glm::vec3(1.f, 1.f, 1.f);
		Light.Ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	}

	if (IsCompatibleWith(1, 7))
	{
		stream->Read(&Light.Intensity, sizeof(float));
	}
	else
	{
		Light.Intensity = 1.f;
	}

	float x = std::cos(glm::radians((float)Light.Longitude)) * std::cos(glm::radians((float)Light.Latitude));
	float y = std::cos(glm::radians((float)Light.Longitude)) * std::sin(glm::radians((float)Light.Latitude));
	float z = std::sin(glm::radians((float)Light.Latitude));

	Light.Position = glm::vec3(-x, y, z);

	if (IsCompatibleWith(1, 6))
	{
		stream->Read(&_Ground, sizeof(GroundInfo));
	}
	else
	{
		_Ground.Top = -500;
		_Ground.Bottom = 500;
		_Ground.Left = -500;
		_Ground.Right = 500;
	}

	int objectCount;
	stream->Read(&objectCount, sizeof(int));
	for (int i = 0; i < objectCount; i++)
	{
		int type, skip = 0;
		stream->Read(&type, sizeof(int));

		if(type == 1)
		{
			WorldModel *model = new WorldModel(this);

			if (!model->Load(stream, header.Version.Major, header.Version.Minor))
				return false;
		}
		else if (type == 2)
		{
			skip = 80 + sizeof(float) * 3 + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(float);
		}
		else if (type == 3)
		{
            skip = 80 + 80 + sizeof(float) * 3 + sizeof(float) + sizeof(int) + sizeof(int) + sizeof(float);

			if (IsCompatibleWith(2, 0))
                skip += sizeof(float);
		}
		else if (type == 4)
		{
			skip = 80 + sizeof(float) * 3 + sizeof(int) + sizeof(float) + sizeof(float) * 4;
		}
		else
		{
			return false;
		}

		if (skip > 0)
			stream->Seek(1, skip);
	}

	LOG("Loaded %d objects", SceneRoot->Children.size());
	
	if (!SetupWaterVertices())
		return false;

	return true;
#undef IsCompatibleWith
}

void World::Update(double elapsed)
{
    _WaterElapsed += elapsed;
	while (_WaterElapsed >= _Water.AnimSpeed * 10)
    {
		_WaterTextureIndex++;

		if (_WaterTextureIndex >= _WaterTextures.size())
            _WaterTextureIndex = 0;

        _WaterElapsed -= _Water.AnimSpeed * 10;
    }

	SceneManager::Update(elapsed);
}

void World::RenderStaticGeometry(double elapsed)
{
	glEnable(GL_DEPTH_TEST);

	/* Ground */
	{
		_GroundShader->Begin();
		_GroundShader->SetUniform("InTexture", 0);
		_GroundShader->SetUniform("InLightmap", 1);
		_GroundShader->SetUniform("ViewProjection", SceneCamera->GetProjection() * SceneCamera->GetView());
		_GroundShader->SetUniform("AmbientColor", _Light.Ambient * _Light.Intensity);
		_GroundShader->SetUniform("DiffuseColor", _Light.Diffuse);
		_GroundShader->SetUniform("LightPosition", _Light.Position);
		_Lightmap->Bind(1);
		_GroundVBuffer->Bind();
		for (unsigned int i = 0; i < _GroundTextures.size(); i++)
		{
			_GroundTextures[i]->Bind(0);
			_GroundVBuffer->Render(GL_TRIANGLES, _GroundIBuffers[i], _GroundIBuffers[i]->Count);
		}
		_GroundShader->End();
	}

	/* Water */
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_CommonShader->Begin();
		_CommonShader->SetUniform("InTexture", 0);
		_CommonShader->SetUniform("Alpha", 0.5f);
		_CommonShader->SetUniform("WorldViewProjection", SceneCamera->GetProjection() * SceneCamera->GetView());
		_CommonShader->SetUniform("AmbientColor", _Light.Ambient * _Light.Intensity);
		_CommonShader->SetUniform("DiffuseColor", _Light.Diffuse);
		_CommonShader->SetUniform("LightPosition", _Light.Position);
		_WaterTextures[_WaterTextureIndex]->Bind(0);
		_WaterVBuffer->Bind();
		_WaterVBuffer->Render(GL_TRIANGLE_STRIP, _WaterIBuffer, _WaterIBuffer->Count);
		_CommonShader->End();

		glDisable(GL_BLEND);
	}

	glDisable(GL_DEPTH_TEST);
}

template<>
std::shared_ptr<World> ContentManager::LoadNew(const std::string &name, bool async)
{
	FilePtr rswFile = FileManager::Instance()->OpenFile(name);
	
	if (!rswFile)
	{
		LOG("Failed loading '%s': file not found", name.c_str());
		return NULL;
	}

	WorldPtr ptr = std::make_shared<World>();

	if (!ptr->LoadFromRsw(rswFile))
	{
		LOG("Error loading '%s'", name.c_str());
		return NULL;
	}

	LOG("Loaded '%s'", name.c_str());

	return ptr;
}
