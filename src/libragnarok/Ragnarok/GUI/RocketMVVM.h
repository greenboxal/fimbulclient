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

#ifndef ROCKETMVVM_H
#define ROCKETMVVM_H

#include <map>
#include <string>
#include <functional>
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>

namespace RocketMVVM
{
	using namespace Awesomium;

	class ViewModel
	{
	public:
		ViewModel()
		{
			
		}

		virtual ~ViewModel()
		{

		}

		void SetValue(const std::string &name, const JSValue &value)
		{
			_Object.SetProperty(ToWebString(name), value);
			_Dirty = true;
		}

		void SetValue(const WebString &name, const JSValue &value)
		{
			_Object.SetProperty(name, value);
			_Dirty = true;
		}

		void SetProxy(const JSValue proxy)
		{
			_Proxy = proxy.ToObject();
		}

		void Update()
		{
			if (_Dirty)
			{
				WebString update(((const wchar16 *)L"update"));

				if (!_Proxy.HasMethod(update))
					return;

				JSArray args;
				args.Push(_Object);
				_Proxy.Invoke(update, args);
				_Dirty = false;
			}
		}

	private:
		bool _Dirty;
		JSObject _Proxy;
		JSObject _Object;
	};
}

#define MVVM_FIELD(name, target, type) \
void Set##name(const Awesomium::JSValue &value) \
{ \
	SetValue(Awesomium::WebString((const wchar16 *)L##target), value); \
} \
void Set##name(const type &value) \
{ \
	Set##name(Awesomium::JSValue(value)); \
}

#define MVVM_FIELD_STR(name, target) \
void Set##name(const Awesomium::JSValue &value) \
{ \
	SetValue(Awesomium::WebString((const wchar16 *)L##target), value); \
} \
void Set##name(const std::string &value) \
{ \
	Set##name(Awesomium::JSValue(Awesomium::ToWebString(value))); \
} \
void Set##name(const char *value) \
{ \
	Set##name(Awesomium::JSValue(Awesomium::WSLit(value))); \
}

#define MVVM_FIELD_NOTYPE(name, target) \
void Set##name(const Awesomium::JSValue &value) \
{ \
	SetValue(Awesomium::WebString((const wchar16 *)L##target), value); \
}

#endif
