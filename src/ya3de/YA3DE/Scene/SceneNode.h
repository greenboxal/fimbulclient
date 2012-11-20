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

#ifndef YA3DE_SCENENODE_H
#define YA3DE_SCENENODE_H

#include <list>

#include <YA3DE/Helpers.h>
#include <YA3DE/Scene/Camera.h>

namespace YADE
{
	class SceneNode
	{
	public:
		SceneNode(SceneNode *parent);
		~SceneNode();

		virtual bool IsVisible(Camera *camera);
		virtual void Update(Camera *camera, double elapsed);
		virtual void Render(Camera *camera, double elapsed);
		void Clear();

		DEFPROP_RO_P(public, SceneNode, Parent);
		DEFPROP_RO_RC(public, std::list<SceneNode *>, Children);
	};
}

#endif
