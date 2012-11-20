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

#ifndef YA3DE_INDEXBUFFER_H
#define YA3DE_INDEXBUFFER_H

#include <memory>

#include <YA3DE/OpenGL.h>
#include <YA3DE/Helpers.h>

namespace YADE
{
	class IndexBuffer
	{
	public:
		IndexBuffer(int type)
		{
			glGenBuffers(1, &_ID);

			_GLType = type;
		}

		~IndexBuffer()
		{
			glDeleteBuffers(1, &_ID);
		}

		int GetSize()
		{
			switch (_GLType)
			{
			case GL_UNSIGNED_BYTE:
				return 1;
			case GL_UNSIGNED_SHORT:
				return 2;
			case GL_UNSIGNED_INT:
				return 4;
			}

			return 0;
		}

		void Bind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID);
		}

		void SetData(void *data, int count, int hint)
		{
			_Count = count;

			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * GetSize(), data, hint);
		}
			
		DEFPROP_RO(public, unsigned int, ID);
		DEFPROP_RO(public, int, GLType);
		DEFPROP_RO(public, int, Count);
	};
	typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
}

#endif
