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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>
#include <YA3DE/Math/Frustum.h>

#include <glm/gtc/matrix_transform.hpp>

#include "FimbulwinterClient.h"

class Camera
{
public:
	Camera(const glm::vec3 &position, const glm::vec3 &target, float nearPlane, float farPlane)
		: _Position(position), _Target(target)
	{
		if (_Position == _Target)
			_Target.z += 10.f;

		_NearPlane = nearPlane;
		_FarPlane = farPlane;

		CalculateYawPitch();

		//while (std::abs(_Pitch) >= glm::radians(80.f))
		//{
		//	_Position.z += 10.f;
		//	CalculateYawPitch();
		//}

		_Projection = glm::perspectiveFov<float>(45.f, (float)Ragnarok->Window->getSize().x, (float)Ragnarok->Window->getSize().y, nearPlane, farPlane);
		_dirty = true;
	}

	void Update()
	{
		if (_dirty)
		{
			_View = glm::lookAt(_Position, _Target, Up);
			_ViewProjection = _Projection * _View;
			_dirty = false;
			_Frustum.Update(_View, _Projection);
		}
	}

	void MoveForward(float amount)
    {
        _Position += amount * Direction;
        _Target += amount * Direction;

		_dirty = true;
    }

    void Strafe(float amount)
    {
        _Position += amount * Right;
        _Target += amount * Right;

		_dirty = true;
    }

    void AddYaw(float angle)
    {
        _Yaw += angle;

        glm::vec4 dir = glm::vec4(Direction, 1);
		dir = glm::rotate(glm::mat4(), angle, Up) * dir;

        _Target = _Position + glm::distance(_Target, _Position) * dir.xyz;
        CalculateYawPitch();
		
		_dirty = true;
    }

    void AddPitch(float angle)
    {
		if (std::abs(_Pitch + angle) >= 80.f) 
			return;

        _Pitch += angle;

        glm::vec4 dir = glm::vec4(Direction, 1);
		dir = glm::rotate(glm::mat4(), angle, Right) * dir;

		_Target = _Position + glm::distance(_Target, _Position) * dir.xyz;
        CalculateYawPitch();

		_dirty = true;
    }

    void Levitate(float amount)
    {
        _Position += Up * amount;
        _Target += Up * amount ;

		_dirty = true;
    }

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
	
	DEFPROP_RO_R(public, glm::mat4, View);
	DEFPROP_RO_R(public, glm::mat4, Projection);
	DEFPROP_RO_R(public, glm::mat4, ViewProjection);
	DEFPROP_RO_R(public, glm::vec3, Position);
	DEFPROP_RO_R(public, glm::vec3, Target);
	DEFPROP_RO(public, float, NearPlane);
	DEFPROP_RO(public, float, FarPlane);
	DEFPROP_RO(public, float, Yaw);
	DEFPROP_RO(public, float, Pitch);
	
	DEFPROP_RO_R(public, YA3DE::Math::Frustum, Frustum);

private:
	void CalculateYawPitch()
    {
        glm::vec3 dir = Direction;

        glm::vec3 m = dir;
		m.y = _Position.y;

        _Yaw = std::atan2(dir.x, dir.z);
        _Pitch = std::atan2(dir.y, (float)glm::vec2(m.x, m.z).length());

		_dirty = true;
    }

	bool _dirty;
};

#endif
