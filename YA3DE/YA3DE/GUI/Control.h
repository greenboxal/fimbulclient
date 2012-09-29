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

#ifndef _GUICONTROL_H_
#define _GUICONTROL_H_

#include <memory>
#include <list>
#include <YA3DE/Math.h>
#include <YA3DE/Graphics/Font.h>

using namespace YA3DE::Graphics;

namespace YA3DE
{
	namespace GUI
	{
		class Control;
		class IGUIRenderer;
		typedef std::shared_ptr<Control> ControlPtr;
		class Control : public std::enable_shared_from_this<Control>
		{
		public:
			Control();
			virtual ~Control();

			virtual std::string GetClass()
			{
				return "YA3DE::GUI::Control";
			}

			int GetHandle() const
			{
				return _handle;
			}

			IGUIRenderer *GetRenderer();

			ControlPtr GetParent()
			{
				return _parent;
			}

			void SetParent(ControlPtr parent);

			void AddChild(ControlPtr control)
			{
				control->SetParent(this->shared_from_this());
				_childs.push_back(control);
			}

			void RemoveChild(ControlPtr control)
			{
				std::list<ControlPtr>::iterator it;

				for (it = _childs.begin(); it != _childs.end(); it++)
					if ((*it)->GetHandle() == control->GetHandle())
						break;

				if (it != _childs.end())
					_childs.erase(it);
			}

			std::list<ControlPtr> &GetChilds()
			{
				return _childs;
			}
			
			glm::vec4 GetForeColor()
			{
				return _foreColor;
			}

			void SetForeColor(glm::vec4 &color)
			{
				_foreColor = color;
			}
			
			glm::vec4 GetBackColor()
			{
				return _backColor;
			}

			void SetBackColor(glm::vec4 &color)
			{
				_backColor = color;
			}
			
			std::string GetText()
			{
				return _text;
			}

			void SetText(std::string text)
			{
				_text = text;
			}

			FontPtr GetFont()
			{
				return _font;
			}

			void SetFont(FontPtr font)
			{
				_font = font;
			}

			int GetFontHeight()
			{
				return _fontHeight;
			}

			void SetFontHeight(int fontHeight)
			{
				_fontHeight = fontHeight;
			}

			glm::uvec2 GetPosition()
			{
				return _position;
			}

			void SetPosition(glm::uvec2 &position)
			{
				_position = position;
			}
			
			glm::uvec2 GetSize()
			{
				return _size;
			}

			void SetSize(glm::uvec2 &size)
			{
				_size = size;
			}

			virtual void Update(double elapsedTime)
			{

			}

		private:
			IGUIRenderer *_renderer;
			ControlPtr _parent;
			std::list<ControlPtr> _childs;
			glm::vec4 _foreColor;
			glm::vec4 _backColor;
			std::string _text;
			FontPtr _font;
			int _fontHeight;
			glm::uvec2 _position;
			glm::uvec2 _size;
			int _handle;
		};
	}
}

#endif
