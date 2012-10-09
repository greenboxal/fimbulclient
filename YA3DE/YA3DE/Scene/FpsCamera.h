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

#ifndef _FPSCAMERA_H_
#define _FPSCAMERA_H_

#include <YA3DE/Scene/Camera.h>

namespace YA3DE
{
	namespace Scene
	{
		class FpsCamera : public Camera
		{
		public:
			FpsCamera(const glm::vec3 &position, const glm::vec3 &target, float nearPlane, float farPlane);
			
			virtual void Update(double elapsed);

			virtual const glm::mat4 &GetView();
			virtual const glm::mat4 &GetProjection();

			void MoveForward(float amount);
			void Strafe(float amount);
			void AddYaw(float angle);
			void AddPitch(float angle);
			void Levitate(float amount);
			
			DEFPROP_SELF_RO(public, glm::vec3, Direction)
			{
				return glm::normalize(_Target - _Position);
			}

			DEFPROP_SELF_RO(public, glm::vec3, Up)
			{
				return glm::vec3(0, 1, 0);
			}

			DEFPROP_SELF_RO(public, glm::vec3, Right)
			{
				return glm::normalize(glm::cross(Direction, Up));
			}
	
			DEFPROP_RO_R(public, glm::vec3, Position);
			DEFPROP_RO_R(public, glm::vec3, Target);
			DEFPROP_RO(public, float, NearPlane);
			DEFPROP_RO(public, float, FarPlane);
			DEFPROP_RO(public, float, Yaw);
			DEFPROP_RO(public, float, Pitch);

		private:
			void CalculateYawPitch();

			bool _Dirty;
			glm::mat4 _View;
			glm::mat4 _Projection;
		};
	}
}

#endif
