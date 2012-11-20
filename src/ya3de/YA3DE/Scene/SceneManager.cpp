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

#include <YA3DE/Scene/SceneManager.h>

using namespace YADE;

SceneManager::SceneManager()
{
	_SceneRoot = new SceneNode(NULL);
	_SceneCamera = NULL;
}

SceneManager::~SceneManager()
{
	delete _SceneRoot;
}

void SceneManager::Update(double elapsed)
{
	if (_SceneCamera != NULL)
		_SceneCamera->Update(elapsed);

	_SceneRoot->Update(_SceneCamera, elapsed);
}

void SceneManager::Render(double elapsed)
{
	RenderStaticGeometry(elapsed);
	
	_SceneRoot->Render(_SceneCamera, elapsed);
}

void SceneManager::RenderStaticGeometry(double elapsed)
{

}
