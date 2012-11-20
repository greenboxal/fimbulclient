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

#ifndef YA3DE_CAMERA_H
#define YA3DE_CAMERA_H

#include <YA3DE/Helpers.h>
#include <YA3DE/Math.h>
#include <YA3DE/Math/Frustum.h>

namespace YADE
{
	class Camera
	{
	public:
		virtual void Update(double elapsed) = 0;

		virtual const glm::mat4 &GetView() = 0;
		virtual const glm::mat4 &GetProjection() = 0;

		DEFPROP_RO(public, Frustum, ViewFrustum);
	};
}

#endif
