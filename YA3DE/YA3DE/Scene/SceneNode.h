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

#pragma once

#include <list>

#include <YA3DE/Helpers.h>
#include <YA3DE/Scene/Camera.h>

namespace YA3DE
{
	namespace Scene
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

			DEFPROP_RO(public, SceneNode *, Parent);
			DEFPROP_RO(public, std::list<SceneNode *>, Children);
		};
	}
}

