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

#ifndef _ROLABELRENDERER_H_
#define _ROLABELRENDERER_H_

#include <YA3DE/GUI/GUIManager.h>
#include <YA3DE/GUI/Label.h>

using namespace YA3DE::GUI;

namespace ROGUI
{
	namespace Renderers
	{
		class ROLabelRenderer : public IGUIRenderer
		{
		public:
			ROLabelRenderer(Label *label)
			{
				_label = label;
			}

			void Render(Graphics2D &g)
			{
				g.Draw(_label->GetFont(), _label->GetFontHeight(), _label->GetText(), glm::uvec2(0), _label->GetForeColor());
			}

		private:
			Label *_label;
		};

		class ROLabelRendererFactory : public IGUIRendererFactory
		{
		public:
			virtual std::string GetName()
			{
				return "YA3DE::GUI::Label";
			}

			virtual IGUIRenderer *CreateRenderer(Control *control)
			{
				return new ROLabelRenderer((Label *)control);
			}
		};
	}
}

#endif
