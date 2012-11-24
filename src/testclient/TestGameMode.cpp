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

#include "RagnarokClient.h"
#include "TestGameMode.h"

#include <YA3DE/Logger.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/Content/StringResource.h>

using namespace YADE;
using namespace Ragnarok;
using namespace Awesomium;
using namespace testclient;

TestGameMode::TestGameMode()
{
	_Camera = new FpsCamera(glm::vec3(800.f, 1350.f, 2600.f), glm::vec3(820.f, -1000.f, 120.f), 1.f, 5000.f);
	_RightWasPressed = false;
}

TestGameMode::~TestGameMode()
{
	delete _Camera;
}

void TestGameMode::Load()
{	
	RagnarokClient *roinstance = RagnarokClient::Instance();
	GuiManager *gui = roinstance->Gui();

	LOG("TestMode::OnLoad");
	_testModeBridge = gui->View()->CreateGlobalJavascriptObject(WSLit("Application.TestMode")).ToObject();

	gui->Dispatcher()->BindAsync(_testModeBridge, "SetMapInfoProxy", JSDispatcher::AsyncFunction([this] (const JSArray &args) {
		if (args.size() != 1)
			return;

		_mapInfoViewModel.SetProxy(args[0]);
	}));

	roinstance->Gui()->SetDesktop("testmode");

	_World = ContentManager::Instance()->Load<World>("data/" + roinstance->Settings().get<std::string>("config.ragnarok.startmap", "prontera") + ".rsw");
	_World->SceneCamera(_Camera);
}

void TestGameMode::OnEvent(const sf::Event &e, double elapsed)
{

}

void TestGameMode::Update(double elapsed)
{
	RagnarokClient *roinstance = RagnarokClient::Instance();
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	float timeDifference = (float)elapsed;

	if (roinstance->IsActive() && roinstance->Gui()->TestPixel(mousePos.x - roinstance->Window().getPosition().x, mousePos.y - roinstance->Window().getPosition().y))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            _Camera->MoveForward(1.0f * timeDifference);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            _Camera->MoveForward(-1.0f * timeDifference);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            _Camera->Strafe(-1.0f * timeDifference);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            _Camera->Strafe(1.0f * timeDifference);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
            _Camera->Levitate(1.0f * timeDifference);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
            _Camera->Levitate(-1.0f * timeDifference);
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            _Camera->AddYaw(1.0f * timeDifference / 10.f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            _Camera->AddYaw(-1.0f * timeDifference / 10.f);
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            _Camera->AddPitch(1.0f * timeDifference / 10.f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            _Camera->AddPitch(-1.0f * timeDifference / 10.f);
		
		bool rightIsPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		if (rightIsPressed && _RightWasPressed)
        {
			sf::Vector2i center = roinstance->Window().getPosition() + sf::Vector2i(roinstance->Window().getSize().x / 2, roinstance->Window().getSize().y / 2);
			
            _Camera->AddYaw((center.x - mousePos.x) / 100.f * timeDifference);
            _Camera->AddPitch((center.y - mousePos.y) / 100.f * timeDifference);
			
            sf::Mouse::setPosition(center);
        }
		else if (rightIsPressed && !_RightWasPressed)
        {
			sf::Vector2i center = roinstance->Window().getPosition() + sf::Vector2i(roinstance->Window().getSize().x / 2, roinstance->Window().getSize().y / 2);

			_PrevMousePos = sf::Mouse::getPosition();
			
			sf::Mouse::setPosition(sf::Vector2i(center.x, center.y));
        }
		else if (!rightIsPressed && _RightWasPressed)
        {
			sf::Mouse::setPosition(_PrevMousePos);
        }

		_RightWasPressed = rightIsPressed;
	}

	_mapInfoViewModel.SetCameraX(_Camera->Position().x);
	_mapInfoViewModel.SetCameraY(_Camera->Position().y);
	_mapInfoViewModel.SetCameraZ(_Camera->Position().z);
	_mapInfoViewModel.SetTargetX(_Camera->Target().x);
	_mapInfoViewModel.SetTargetY(_Camera->Target().y);
	_mapInfoViewModel.SetTargetZ(_Camera->Target().z);
	_mapInfoViewModel.Update();

	_World->Update(elapsed);
}

void TestGameMode::Render(double elapsed)
{
	_World->Render(elapsed);
}

void TestGameMode::Unload()
{
	LOG("TestMode::OnUnload");
}
