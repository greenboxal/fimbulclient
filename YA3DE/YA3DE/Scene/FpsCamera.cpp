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

#include <stdafx.h>
#include <YA3DE/Game.h>
#include <YA3DE/Scene/FpsCamera.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace YA3DE;
using namespace YA3DE::Scene;

FpsCamera::FpsCamera(const glm::vec3 &position, const glm::vec3 &target, float nearPlane, float farPlane)
{
	if (_Position == _Target)
		_Target.z += 10.f;

	_NearPlane = nearPlane;
	_FarPlane = farPlane;

	CalculateYawPitch();

	_Projection = glm::perspectiveFov<float>(45.f, (float)Game::Instance()->Window.Size.x, (float)Game::Instance()->Window.Size.y, nearPlane, farPlane);
	_Dirty = true;
}

void FpsCamera::Update(double elapsed)
{
	if (_Dirty)
	{
		_View = glm::lookAt(_Position, _Target, Up);
		_Dirty = false;
		Frustum.Update(_View, _Projection);
	}
}

const glm::mat4 &FpsCamera::GetView()
{
	return _View;
}

const glm::mat4 &FpsCamera::GetProjection()
{
	return _Projection;
}

void FpsCamera::MoveForward(float amount)
{
    _Position += amount * Direction;
    _Target += amount * Direction;

	_Dirty = true;
}

void FpsCamera::Strafe(float amount)
{
    _Position += amount * Right;
    _Target += amount * Right;

	_Dirty = true;
}

void FpsCamera::AddYaw(float angle)
{
    _Yaw += angle;

    glm::vec4 dir = glm::vec4(Direction, 1);
	dir = glm::rotate(glm::mat4(), angle, Up) * dir;

    _Target = _Position + glm::distance(_Target, _Position) * dir.xyz;
    CalculateYawPitch();
		
	_Dirty = true;
}

void FpsCamera::AddPitch(float angle)
{
    _Pitch += angle;

    glm::vec4 dir = glm::vec4(Direction, 1);
	dir = glm::rotate(glm::mat4(), angle, Right) * dir;

	_Target = _Position + glm::distance(_Target, _Position) * dir.xyz;
    CalculateYawPitch();

	_Dirty = true;
}

void FpsCamera::Levitate(float amount)
{
    _Position += Up * amount;
    _Target += Up * amount ;

	_Dirty = true;
}

void FpsCamera::CalculateYawPitch()
{
    glm::vec3 dir = Direction;

    glm::vec3 m = dir;
	m.y = _Position.y;

    _Yaw = std::atan2(dir.x, dir.z);
    _Pitch = std::atan2(dir.y, (float)glm::vec2(m.x, m.z).length());

	_Dirty = true;
}
