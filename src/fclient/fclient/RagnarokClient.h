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

#ifndef FCLIENT_RAGNAROKCLIENT_H
#define FCLIENT_RAGNAROKCLIENT_H

#include <fclient/GuiManager.h>
#include <fclient/GameMode.h>

#include <YA3DE/Application.h>
#include <YA3DE/Graphics/Texture2D.h>

#include <Awesomium/WebCore.h>

#define Ragnarok ((fclient::RagnarokClient *)YADE::Application::Instance())

namespace fclient
{
	using namespace YADE;

	class RagnarokClient : public Application
	{
	public:
		RagnarokClient();
		virtual ~RagnarokClient();

		void ChangeGameMode(GameMode *mode);

		DEFPROP_RO_P(public, GuiManager, Gui);

	protected:
		virtual void Initialize();
		virtual void Load();
		virtual void OnEvent(const sf::Event &e, double elapsed);
		virtual void Update(double elapsed);
		virtual void Render(double elapsed);
		virtual void Unload();

	private:
		GameMode *_CurrentGameMode;
	};
}

#endif
