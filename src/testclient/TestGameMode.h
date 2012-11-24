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

#ifndef FCLIENT_TESTGAMEMODE_H
#define FCLIENT_TESTGAMEMODE_H

#include <YA3DE/Scene/FpsCamera.h>
#include <Ragnarok/GUI/RocketMVVM.h>
#include <Ragnarok/Graphics/World.h>

#include "GameMode.h"

namespace testclient
{
	using namespace YADE;
	using namespace Ragnarok;

	class MapInfoViewModel : public RocketMVVM::ViewModel
	{
	public:
		MVVM_FIELD_STR(MapName, "mapName")
		MVVM_FIELD(CameraX, "cameraX", float)
		MVVM_FIELD(CameraY, "cameraY", float)
		MVVM_FIELD(CameraZ, "cameraZ", float)
		MVVM_FIELD(TargetX, "targetX", float)
		MVVM_FIELD(TargetY, "targetY", float)
		MVVM_FIELD(TargetZ, "targetZ", float)
	};

	class TestGameMode : public GameMode
	{
	public:
		TestGameMode();
		virtual ~TestGameMode();

		virtual void Load();
		virtual void OnEvent(const sf::Event &e, double elapsed);
		virtual void Update(double elapsed);
		virtual void Render(double elapsed);
		virtual void Unload();

	private:
		// GUI Interaction
		Awesomium::JSObject _testModeBridge;
		MapInfoViewModel _mapInfoViewModel;

		FpsCamera *_Camera;
		WorldPtr _World;
		bool _RightWasPressed;
		sf::Vector2i _PrevMousePos;
	};
}

#endif
