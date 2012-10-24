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
#include <YA3DE/Scene/SceneManager.h>

using namespace YA3DE::Scene;

SceneNode::SceneNode(SceneNode *parent)
{
	_Parent = parent;
	if (_Parent != NULL)
		_Parent->Children.push_back(this);
}

SceneNode::~SceneNode()
{
	Clear();
}

bool SceneNode::IsVisible(Camera *camera)
{
	return true;
}

void SceneNode::Update(Camera *camera, double elapsed)
{
	for (SceneNode *node : _Children)
		node->Update(camera, elapsed);
}

void SceneNode::Render(Camera *camera, double elapsed)
{
	for (SceneNode *node : _Children)
		if (node->IsVisible(camera))
			node->Render(camera, elapsed);
}

void SceneNode::Clear()
{
	for (SceneNode *node : _Children)
		delete node;
	
	_Children.clear();
}
