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

#pragma once

#include <assert.h>

#include <string>
#include <list>

#include <YA3DE/Helpers.h>
#include <YA3DE/Graphics/Shader.h>
#include <YA3DE/Content/Resource.h>

namespace YA3DE
{
	namespace Graphics
	{
		class ShaderProgram : public YA3DE::Content::Resource
		{
		public:
			ShaderProgram();
			~ShaderProgram();

			void AddShader(ShaderPtr shader);

			bool Link();

			void Begin();
			int GetUniformID(const std::string &name);
			void End();

			template<typename T>
			void SetUniform(const std::string &name, const T &value)
			{
				int index = GetUniformID(name);

#ifdef _DEBUG
				if (index == -1)
					return;
#else
				assert(index != -1);
#endif

				SetUniform<T>(index, value);
			}
			
			template<typename T>
			void SetUniform(int id, const T &value);
			
			DEFPROP_RO(public, unsigned int, ID);
			DEFPROP_RO_R(public, std::list<ShaderPtr>, Shaders);
		};
		typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
	}
}

