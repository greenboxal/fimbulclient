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

#ifndef FCLIENT_GAMEMODE_H
#define FCLIENT_GAMEMODE_H

#include <map>
#include <YA3DE/Application.h>
#include <Awesomium/WebCore.h>

namespace testclient
{
	class SubGameMode : public Awesomium::JSMethodHandler
	{
	public:
		virtual ~SubGameMode() { }
		virtual void Load() = 0;
		virtual void OnEvent(const sf::Event &e, double elapsed) = 0;
		virtual void Update(double elapsed) = 0;
		virtual void Render(double elapsed) = 0;
		virtual void Unload() = 0;

		virtual void OnMethodCall(Awesomium::WebView* caller,
						unsigned int remote_object_id, 
						const Awesomium::WebString& method_name,
						const Awesomium::JSArray& args)
		{

		}

		virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller,
						unsigned int remote_object_id,
						const Awesomium::WebString& method_name,
						const Awesomium::JSArray& args)
		{
			return Awesomium::JSValue::Undefined();
		}
	};

	class GameMode
	{
	public:
		GameMode();
		virtual ~GameMode();

		virtual void Load();
		virtual void OnEvent(const sf::Event &e, double elapsed);
		virtual void Update(double elapsed);
		virtual void Render(double elapsed);
		virtual void Unload();

		void ChangeSubMode(size_t mode);

	protected:
		void RegisterSubMode(size_t modeId, SubGameMode *mode);

	private:
		std::map<size_t, SubGameMode *> _SubGameModes;
		SubGameMode *_ActiveSubMode;
	};
}

#endif
