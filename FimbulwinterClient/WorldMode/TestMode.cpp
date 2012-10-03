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

#include "TestMode.h"
#include "../FileSystem/Grf/Grf.h"
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/Content/StringResource.h>
#include "../Graphics/World.h"

using namespace WorldMode;
using namespace ROGraphics;
using namespace YA3DE::Content;

TestMode::TestMode()
	: _FPSCamera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), 1.f, 5000.f)
{
	prevRightButton = false;
}

TestMode::~TestMode()
{

}

WorldPtr world;
void TestMode::OnLoad()
{
	world = ContentManager::Instance()->Load<World>("data/prt_in.rsw");
}

void TestMode::Update(double elapsed)
{
	float timeDifference = (float)elapsed;

	if (Ragnarok->IsActive)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            _FPSCamera.MoveForward(1.0f * timeDifference);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            _FPSCamera.MoveForward(-1.0f * timeDifference);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            _FPSCamera.Strafe(-1.0f * timeDifference);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            _FPSCamera.Strafe(1.0f * timeDifference);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            _FPSCamera.Levitate(1.0f * timeDifference);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            _FPSCamera.Levitate(-1.0f * timeDifference);

		sf::FloatRect view = Ragnarok->Window->getView().getViewport();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && prevRightButton)
        {
			sf::Vector2i center = Ragnarok->Window->getPosition() + sf::Vector2i(Ragnarok->Window->getSize().x / 2, Ragnarok->Window->getSize().y / 2);

            _FPSCamera.AddYaw((center.x - mousePos.x) / 100.0F * timeDifference);
            _FPSCamera.AddPitch((center.y - mousePos.y) / 100.0F * timeDifference);
			
            sf::Mouse::setPosition(center);
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !prevRightButton)
        {
			sf::Vector2i center = Ragnarok->Window->getPosition() + sf::Vector2i(Ragnarok->Window->getSize().x / 2, Ragnarok->Window->getSize().y / 2);

            tempMousePosition = mousePos;

            sf::Mouse::setPosition(center);
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && prevRightButton)
        {
            sf::Mouse::setPosition(tempMousePosition);
        }

		prevRightButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);

		_FPSCamera.Update();

		char buffer[1024];
		sprintf(buffer, "X=%f, Y=%f, Z=%f -> X=%f, Y=%f, Z=%f", _FPSCamera.Position.x, _FPSCamera.Position.y, _FPSCamera.Position.z, _FPSCamera.Target.x, _FPSCamera.Target.y, _FPSCamera.Target.z);
		Ragnarok->Window->setTitle(buffer);
	}

	world->Update(elapsed);
}

void TestMode::Render(double elapsed)
{
	world->Render(_FPSCamera, elapsed);
}

void TestMode::OnEvent(sf::Event &ev, double elapsedTime)
{

}

void TestMode::OnUnload()
{

}
