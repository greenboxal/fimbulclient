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

#ifndef _FRUSTUM_H_
#define _FURSTUM_H_

#include <glm/gtc/matrix_access.hpp>

#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>
#include <YA3DE/Math/AABBox.h>

namespace YA3DE
{
	namespace Math
	{
		class Frustum
		{
		public:
			bool IsBoxInside(AABBox &box, glm::mat4 &model)
			{
				if (!_Culling)
					return true;
				
				glm::vec3 min = (model * glm::vec4(box.Min, 1.f)).xyz;
				glm::vec3 max = (model * glm::vec4(box.Max, 1.f)).xyz;
				
				for (int i = 0; i < 6; i++)
				{
					if(_Frustum[i].x * min.x + _Frustum[i].y * min.y + _Frustum[i].z * min.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * max.x + _Frustum[i].y * min.y + _Frustum[i].z * min.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * min.x + _Frustum[i].y * max.y + _Frustum[i].z * min.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * max.x + _Frustum[i].y * max.y + _Frustum[i].z * min.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * min.x + _Frustum[i].y * min.y + _Frustum[i].z * max.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * max.x + _Frustum[i].y * min.y + _Frustum[i].z * max.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * min.x + _Frustum[i].y * max.y + _Frustum[i].z * max.z + _Frustum[i].w > 0)  continue;
					if(_Frustum[i].x * max.x + _Frustum[i].y * max.y + _Frustum[i].z * max.z + _Frustum[i].w > 0)  continue;

					return false;
				}

				return true;
			}

			void Update(const glm::mat4 &view, const glm::mat4 &proj)
			{
				glm::mat4 clip = proj * view;

				// Right
				_Frustum[0] = normalizePlane(glm::column(clip, 3) - glm::column(clip, 0));

				// Left
				_Frustum[1] = normalizePlane(glm::column(clip, 3) + glm::column(clip, 0));
				
				// Top
				_Frustum[2] = normalizePlane(glm::column(clip, 3) - glm::column(clip, 1));

				// Bottom
				_Frustum[3] = normalizePlane(glm::column(clip, 3) + glm::column(clip, 1));

				// Back
				_Frustum[4] = normalizePlane(glm::column(clip, 3) - glm::column(clip, 2));

				// Front
				_Frustum[5] = normalizePlane(glm::column(clip, 3) + glm::column(clip, 2));
			}

			DEFPROP_RW(public, bool, Culling);

		private:
			glm::vec4 normalizePlane(const glm::vec4 &in)
			{
				return in / std::sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
			}

			glm::vec4 _Frustum[6];
		};
	}
}

#endif
