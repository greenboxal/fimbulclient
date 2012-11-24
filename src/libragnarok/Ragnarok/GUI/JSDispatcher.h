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

#ifndef FCLIENT_JSDISPATCHER_H
#define FCLIENT_JSDISPATCHER_H

#include <map>
#include <string>
#include <functional>
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>

namespace Ragnarok
{
	using namespace Awesomium;

	class JSDispatcher : protected Awesomium::JSMethodHandler
	{
	public:
		typedef std::function<void (const JSArray &arguments)> AsyncFunction;
		typedef std::function<JSValue (const JSArray &arguments)> SyncFunction;

		JSDispatcher(WebView *view)
		{
			_View = view;
			_View->set_js_method_handler(this);
		}

		~JSDispatcher()
		{
			_View->set_js_method_handler(nullptr);
		}

		void BindAsync(JSObject &object, const std::string &name, const AsyncFunction &function)
		{
			BindAsync(object, ToWebString(name), function);
		}

		void BindAsync(JSObject &object, const WebString &name, const AsyncFunction &function)
		{
			object.SetCustomMethod(name, false);
			_Binds[object.remote_id()].AsyncFunctions[name] = function;
		}

		void BindSync(JSObject &object, const std::string &name, const SyncFunction &function)
		{
			BindSync(object, ToWebString(name), function);
		}

		void BindSync(JSObject &object, const WebString &name, const SyncFunction &function)
		{
			object.SetCustomMethod(name, true);
			_Binds[object.remote_id()].SyncFunctions[name] = function;
		}

	protected:		
		virtual void OnMethodCall(WebView *caller, unsigned int remote_object_id,  const WebString &method_name, const JSArray &args)
		{
			std::map<unsigned int, BindedObject>::iterator it = _Binds.find(remote_object_id);

			if (it != _Binds.end())
			{
				std::map<WebString, AsyncFunction>::iterator it2 = it->second.AsyncFunctions.find(method_name);

				if (it2 != it->second.AsyncFunctions.end())
					it2->second(args);
			}
		}

		virtual JSValue OnMethodCallWithReturnValue(WebView *caller, unsigned int remote_object_id, const WebString &method_name, const JSArray &args)
		{
			std::map<unsigned int, BindedObject>::iterator it = _Binds.find(remote_object_id);

			if (it != _Binds.end())
			{
				std::map<WebString, SyncFunction>::iterator it2 = it->second.SyncFunctions.find(method_name);

				if (it2 != it->second.SyncFunctions.end())
					return it2->second(args);
			}

			return Awesomium::JSValue::Undefined();
		}

	private:
		struct BindedObject
		{
			std::map<WebString, AsyncFunction> AsyncFunctions;
			std::map<WebString, SyncFunction> SyncFunctions;
		};

		WebView *_View;
		std::map<unsigned int, BindedObject> _Binds;
	};
}

#endif
