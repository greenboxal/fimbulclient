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

#include "TestMode.h"
#include "../FimbulwinterClient.h"

#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/Content/StringResource.h>

using namespace WorldMode;
using namespace ROGraphics;
using namespace YA3DE::Content;

TestMode::TestMode()
{
	_Camera = new FpsCamera(glm::vec3(800.f, 1350.f, 2600.f), glm::vec3(820.f, -1000.f, 120.f), 1.f, 5000.f);
	prevRightButton = false;
}

TestMode::~TestMode()
{
	delete _Camera;
}

void TestMode::OnLoad()
{
	LOG("TestMode::OnLoad");

	std::string mapName = Ragnarok->Configuration->GetRoot().first_node("Config")->first_node("Ragnarok")->first_attribute("World")->value();

	_World = ContentManager::Instance()->Load<World>("data/" + mapName + ".rsw");
	_World->SceneCamera = _Camera;
}

void TestMode::Update(double elapsed)
{
	float timeDifference = (float)elapsed;

	if (Ragnarok->Window.IsActive)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            _Camera->MoveForward(1.0f * timeDifference);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            _Camera->MoveForward(-1.0f * timeDifference);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            _Camera->Strafe(-1.0f * timeDifference);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            _Camera->Strafe(1.0f * timeDifference);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            _Camera->Levitate(1.0f * timeDifference);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            _Camera->Levitate(-1.0f * timeDifference);
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            _Camera->AddYaw(1.0f * timeDifference / 10.f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            _Camera->AddYaw(-1.0f * timeDifference / 10.f);
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            _Camera->AddPitch(1.0f * timeDifference / 10.f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            _Camera->AddPitch(-1.0f * timeDifference / 10.f);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && prevRightButton)
        {
			glm::uvec2 center = Ragnarok->Window.Position + glm::uvec2(Ragnarok->Window.Size.x / 2, Ragnarok->Window.Size.y / 2);
			
            _Camera->AddYaw((center.x - mousePos.x) / 100.f * timeDifference);
            _Camera->AddPitch((center.y - mousePos.y) / 100.f * timeDifference);
			
            sf::Mouse::setPosition(sf::Vector2i(center.x, center.y));
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !prevRightButton)
        {
			glm::uvec2 center = Ragnarok->Window.Position + glm::uvec2(Ragnarok->Window.Size.x / 2, Ragnarok->Window.Size.y / 2);

            tempMousePosition = mousePos;
			
            sf::Mouse::setPosition(sf::Vector2i(center.x, center.y));
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && prevRightButton)
        {
            sf::Mouse::setPosition(tempMousePosition);
        }

		prevRightButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);

		char buffer[1024];
		sprintf(buffer, "X=%f, Y=%f, Z=%f -> X=%f, Y=%f, Z=%f", _Camera->Position.x, _Camera->Position.y, _Camera->Position.z, _Camera->Target.x, _Camera->Target.y, _Camera->Target.z);
		Ragnarok->Window.SetTitle(buffer);
	}

	_World->Update(elapsed);
}

void TestMode::Render(double elapsed)
{
	_World->Render(elapsed);
}

void TestMode::OnEvent(sf::Event &ev, double elapsedTime)
{

}

void TestMode::OnUnload()
{
	LOG("TestMode::OnUnload");
}
