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

#ifndef _FONT_H_
#define _FONT_H_

#include <string>
#include <list>
#include <vector>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SIZES_H

#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>
#include <YA3DE/Graphics/ShaderProgram.h>
#include <YA3DE/Graphics/VertexPositionTexture.h>

namespace YA3DE
{
	namespace Graphics
	{
		namespace FontStyle
		{
			enum
			{
				None = 0x0,
				Bold = 0x1,
				Italic = 0x2,
				Underline = 0x4,
				Strikeout = 0x8,
			};
		}

		enum class FontLoadPolicy
		{
			Direct,
			Lazy
		};

		struct FontGlyph
		{
			FontGlyph()
			{
				Cached = false;
			}

			VertexPositionTexture Vertices[4];
			FT_Glyph_Metrics Metrics;
			FT_Vector Advance;
			glm::uvec2 AtlasPosition;
			bool Cached;
			int GlyphID;
		};

		class Font : public std::shared_ptr<Font>
		{
		public:
			Font(void *memory, unsigned int mem_size, int size, int style, FontLoadPolicy policy);
			~Font();

			void Render(const std::string &text, const glm::uvec2 &pos, const glm::vec4 &color);
			int Measure(const std::string &text);
			
			const FontGlyph *GetGlyph(char c);
			int GetAdvance(char c, char n);

			DEFPROP_RO(public, std::string, Name);
			DEFPROP_RO(public, int, Height);
			DEFPROP_RO(public, int, Size);
			DEFPROP_RO(public, int, Style);

			DEFPROP_SELF_RO(public, bool, Bold)
			{
				return (_Style & FontStyle::Bold) != 0;
			}

			DEFPROP_SELF_RO(public, bool, Italic)
			{
				return (_Style & FontStyle::Italic) != 0;
			}

			DEFPROP_SELF_RO(public, bool, Underline)
			{
				return (_Style & FontStyle::Underline) != 0;
			}

			DEFPROP_SELF_RO(public, bool, Strikeout)
			{
				return (_Style & FontStyle::Strikeout) != 0;
			}

		private:
			FT_Face _Face;
			ShaderProgramPtr _Shader;
			std::vector<FontGlyph> _Glyphs;
			void *_Memory;

			unsigned int _Texture;
			unsigned char *_Atlas;
			int _AtlasWidth, _AtlasHeight;

			void SetAtlasRegion(int x, int y, int w, int h, unsigned char *data, int pitch);
		};
		typedef std::shared_ptr<Font> FontPtr;
	}
}

#endif
