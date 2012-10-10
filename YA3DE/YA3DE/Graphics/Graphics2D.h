/*	This file is part of YA3DE.

	YA3DE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	YA3DE is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with YA3DE.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _GRAPHICS2D_H_
#define _GRAPHICS2D_H_

#include <YA3DE/Math.h>
#include <YA3DE/Graphics/Font.h>
#include <YA3DE/Graphics/Texture2D.h>

namespace YA3DE
{
	namespace Graphics
	{
		class Graphics2D
		{
		public:
			class Step2D
			{
			public:
				virtual void Draw() { };
			};

			Graphics2D(glm::uvec4 &bounds = glm::uvec4(0))
			{
				_bounds = bounds;
			}

			~Graphics2D()
			{
				std::list<Step2D *>::iterator it;
				for (it = _steps.begin(); it != _steps.end(); it++)
				{
					delete *it;
				}
			}

			void Begin();
			void Draw(Texture2DPtr texture, const glm::uvec4 &rect, const glm::uvec4 &srect, const glm::vec4 &color, const float rotation, const float depth);
			void Draw(FontPtr font, const std::string &str, const glm::uvec2 &pos, const glm::vec4 &color = glm::vec4(1.0f));
			void End();

			void Draw(Texture2DPtr texture, const glm::uvec4 &rect, const glm::vec4 &color)
			{
				Draw(texture, rect, glm::uvec4(0, 0, texture->Width, texture->Height), color, 0, 0);
			}

			void Draw(Texture2DPtr texture, const glm::uvec4 &rect, const glm::uvec4 &srect, const glm::vec4 &color)
			{
				Draw(texture, rect, srect, color, 0, 0);
			}

			void Draw(Texture2DPtr texture, const glm::uvec4 &rect, const glm::uvec4 &srect, const glm::vec4 &color, const float rotation)
			{
				Draw(texture, rect, srect, color, rotation, 0);
			}

		private:
			glm::uvec4 _bounds;
			std::list<Step2D *> _steps;
		};
	}
}

#endif
