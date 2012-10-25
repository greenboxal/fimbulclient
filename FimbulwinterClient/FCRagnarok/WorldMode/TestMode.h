/*	This file is part of FimbulwinterClient.

	FimbulwinterClient is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	FimbulwinterClient is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FimbulwinterClient.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include "../GameMode.h"
#include "../Graphics/World.h"

#include <YA3DE/Scene/FpsCamera.h>

using namespace YA3DE::Scene;
using namespace YA3DE::System;

namespace FCRagnarok
{
	namespace WorldMode
	{
		class TestMode : public SubGameMode
		{
		public:
			TestMode();
			~TestMode();

			virtual void OnLoad();
			virtual void Update(double elapsed);
			virtual void Render(double elapsed);
			virtual void OnEvent(Event &ev, double elapsedTime);
			virtual void OnUnload();

		private:
			FpsCamera *_Camera;
			Graphics::WorldPtr _World;
			MouseState _PrevMouseState;
			glm::uvec2 _PrevMousePos;
		};
	}
}

