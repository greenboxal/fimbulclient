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

#include <stdafx.h>
#include <FCRagnarok/WorldMode/TestMode.h>
#include <FCRagnarok/FimbulwinterClient.h>

#include <YA3DE/System/Mouse.h>
#include <YA3DE/System/Keyboard.h>
#include <YA3DE/Graphics/Graphics2D.h>
#include <YA3DE/Graphics/FontManager.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/Content/StringResource.h>

using namespace FCRagnarok::WorldMode;
using namespace FCRagnarok::Graphics;

using namespace YA3DE::Content;
using namespace YA3DE::Graphics;

TestMode::TestMode()
{
	_Camera = new FpsCamera(glm::vec3(800.f, 1350.f, 2600.f), glm::vec3(820.f, -1000.f, 120.f), 1.f, 5000.f);
}

TestMode::~TestMode()
{
	delete _Camera;
}

void TestMode::OnLoad()
{
	LOG("TestMode::OnLoad");

	_World = ContentManager::Instance()->Load<World>("data/" + Ragnarok->Settings.get<std::string>("config.ragnarok.startmap", "prontera") + ".rsw");
	_World->SceneCamera = _Camera;
}

void TestMode::Update(double elapsed)
{
	float timeDifference = (float)elapsed;

	if (Ragnarok->Window.IsActive)
	{
		MouseState state;
		Mouse::GetState(state);

		if (Keyboard::GetKeyState(Key::W) == KeyState::Pressed)
            _Camera->MoveForward(1.0f * timeDifference);
		else if (Keyboard::GetKeyState(Key::S) == KeyState::Pressed)
            _Camera->MoveForward(-1.0f * timeDifference);

		if (Keyboard::GetKeyState(Key::A) == KeyState::Pressed)
            _Camera->Strafe(-1.0f * timeDifference);
        else if (Keyboard::GetKeyState(Key::D) == KeyState::Pressed)
            _Camera->Strafe(1.0f * timeDifference);

		if (Keyboard::GetKeyState(Key::LShift) == KeyState::Pressed)
            _Camera->Levitate(1.0f * timeDifference);
		else if (Keyboard::GetKeyState(Key::LControl) == KeyState::Pressed)
            _Camera->Levitate(-1.0f * timeDifference);
		
		if (Keyboard::GetKeyState(Key::Left) == KeyState::Pressed)
            _Camera->AddYaw(1.0f * timeDifference / 10.f);
		else if (Keyboard::GetKeyState(Key::Right) == KeyState::Pressed)
            _Camera->AddYaw(-1.0f * timeDifference / 10.f);
		
		if (Keyboard::GetKeyState(Key::Up) == KeyState::Pressed)
            _Camera->AddPitch(1.0f * timeDifference / 10.f);
		else if (Keyboard::GetKeyState(Key::Down) == KeyState::Pressed)
            _Camera->AddPitch(-1.0f * timeDifference / 10.f);

		if (state.RightButton == ButtonState::Pressed && _PrevMouseState.RightButton == ButtonState::Pressed)
        {
			glm::uvec2 center = Ragnarok->Window.Position + glm::uvec2(Ragnarok->Window.Size.x / 2, Ragnarok->Window.Size.y / 2);
			
            _Camera->AddYaw((center.x - state.X) / 100.f * timeDifference);
            _Camera->AddPitch((center.y - state.Y) / 100.f * timeDifference);
			
            Mouse::SetPosition(center);
        }
		else if (state.RightButton == ButtonState::Pressed && _PrevMouseState.RightButton == ButtonState::Released)
        {
			glm::uvec2 center = Ragnarok->Window.Position + glm::uvec2(Ragnarok->Window.Size.x / 2, Ragnarok->Window.Size.y / 2);

            _PrevMousePos = glm::uvec2(state.X, state.Y);
			
            Mouse::SetPosition(center);
        }
		else if (state.RightButton == ButtonState::Released && _PrevMouseState.RightButton == ButtonState::Pressed)
        {
			Mouse::SetPosition(_PrevMousePos);
        }

		_PrevMouseState = state;
	}

	_World->Update(elapsed);
}

void TestMode::Render(double elapsed)
{
	char buffer[1024];
	sprintf(buffer, "ABCabcÁÉÍÂ", _Camera->Position.x, _Camera->Position.y, _Camera->Position.z, _Camera->Target.x, _Camera->Target.y, _Camera->Target.z);

	_World->Render(elapsed);

	Graphics2D g;
	g.Begin();
	g.Draw(FontManager::Instance()->LoadFont("arial", 48, FontStyle::None, FontLoadPolicy::Direct), buffer, glm::uvec2(1, 50), glm::vec4(1.f));
	g.End();
}

void TestMode::OnEvent(Event &ev, double elapsedTime)
{

}

void TestMode::OnUnload()
{
	LOG("TestMode::OnUnload");
}
