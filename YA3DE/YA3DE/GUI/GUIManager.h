/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

#include <map>
#include <list>
#include <YA3DE/GUI/IGUIRenderer.h>

namespace YA3DE
{
	namespace GUI
	{
		class GUIManager
		{
		public:
			GUIManager()
			{
				_handleCounter = 0;
			}

			~GUIManager()
			{
				std::map<std::string, IGUIRendererFactory *>::iterator it;

				for (it = _renderers.begin(); it != _renderers.end(); it++)
					delete it->second;
			}

			IGUIRenderer *CreateRenderer(Control *control)
			{
				std::map<std::string, IGUIRendererFactory *>::iterator it;

				it = _renderers.find(control->GetClass());

				if (it != _renderers.end())
					return it->second->CreateRenderer(control);

				return NULL;
			}

			void RegisterRenderer(IGUIRendererFactory *factory)
			{
				_renderers[factory->GetName()] = factory;
			}

			void AddRootControl(ControlPtr control)
			{
				control->SetParent(NULL);
				_rootWindows[control->GetHandle()] = control;
			}

			void RemoveRootControl(ControlPtr control)
			{
				_rootWindows.erase(control->GetHandle());
			}

			int GetNewHandle()
			{
				return ++_handleCounter;
			}

			ControlPtr GetControlByHandle(int handle)
			{
				std::map<int, ControlPtr>::iterator it = _rootWindows.find(handle);
				return it == _rootWindows.end() ? NULL : it->second;
			}

			FontPtr GetDefaultFont()
			{
				return _defaultFont;
			}

			void SetDefaultFont(FontPtr font)
			{
				_defaultFont = font;
			}

			Graphics2D CreateGraphics(ControlPtr control);
			glm::uvec4 GetAbsoluteControlBound(ControlPtr ptr);

			void UpdateControl(ControlPtr control, double elapsedTime);
			void RenderControl(ControlPtr control, double elapsedTime);

			void Update(double elapsedTime);
			void Render(double elapsedTime);

			static GUIManager *Instance()
			{
				if (_instance == NULL)
					_instance = new GUIManager();

				return _instance;
			}
		private:
			FontPtr _defaultFont;
			int _handleCounter;
			std::map<std::string, IGUIRendererFactory *> _renderers;
			std::map<int, ControlPtr> _rootWindows;

			static GUIManager *_instance;
		};
	}
}

#endif
