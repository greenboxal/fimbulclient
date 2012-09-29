/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _FRUSTUM_H_
#define _FURSTUM_H_

#include <YA3DE/Helpers.h>
#include <YA3DE/Math.h>
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

				glm::mat4 m =_ProjectionMatrix * _ViewMatrix * model;
			}

			DEFPROP_RW_P(public, glm::mat4 &, ProjectionMatrix);
			DEFPROP_RW_P(public, glm::mat4 &, ViewMatrix);
			DEFPROP_RW(public, bool, Culling);
		};
	}
}

#endif
