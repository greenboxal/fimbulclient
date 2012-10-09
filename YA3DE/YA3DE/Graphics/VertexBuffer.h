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

#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include <memory>

#include <YA3DE/OpenGL.h>
#include <YA3DE/Helpers.h>
#include <YA3DE/Graphics/IndexBuffer.h>
#include <YA3DE/Graphics/VertexDeclaration.h>

namespace YA3DE
{
	namespace Graphics
	{
		class VertexBuffer
		{
		public:
			VertexBuffer(VertexDeclaration &declaration)
				: _Declaration(declaration)
			{
				glGenBuffers(1, &_ID);
			}

			~VertexBuffer()
			{
				glDeleteBuffers(1, &_ID);
			}
			
			void Bind()
			{
				glBindBuffer(GL_ARRAY_BUFFER, _ID);
				_Declaration.Activate();
			}

			void SetData(void *data, int count, int hint)
			{
				Bind();
				glBufferData(GL_ARRAY_BUFFER, count * _Declaration.TotalSize, data, hint);
			}

			void Render(int mode, IndexBufferPtr indices, int count)
			{
				indices->Bind();
				glDrawElements(mode, count, indices->GLType, NULL);
			}

			DEFPROP_RO(public, unsigned int, ID);
			DEFPROP_RO(public, int, Type);
			DEFPROP_RO_R(public, VertexDeclaration, Declaration);
		};
		typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
	}
}

#endif
