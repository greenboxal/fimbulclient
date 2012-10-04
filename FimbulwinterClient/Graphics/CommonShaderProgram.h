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

#ifndef _COMMONSHADERPROGRAM_H_
#define _COMMONSHADERPROGRAM_H_

#include <YA3DE/Math.h>
#include <YA3DE/Graphics/ShaderProgram.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/Content/StringResource.h>

using namespace YA3DE::Content;
using namespace YA3DE::Graphics;

namespace ROGraphics
{
	class CommonShaderProgram : public ShaderProgram
	{
	public:
		CommonShaderProgram()
		{
			ShaderPtr watervs(new Shader(GL_VERTEX_SHADER));
			if (!watervs->Load((char *)ContentManager::Instance()->Load<StringResource>("data/shaders/model.glsl")->GetData()))
				return;
			AddShader(watervs);

			ShaderPtr waterps(new Shader(GL_FRAGMENT_SHADER));
			if (!waterps->Load((char *)ContentManager::Instance()->Load<StringResource>("data/shaders/model.glsl")->GetData()))
				return;
			AddShader(waterps);

			if (!Link())
				return;

			_InTexturePosition = GetUniformID("InTexture");
			_AlphaPosition = GetUniformID("Alpha");
			_WVPPosition = GetUniformID("WorldViewProjection");
		}

		void SetTexture(int id)
		{
			if (_InTexturePosition != -1)
				SetUniform(_InTexturePosition, id);
		}

		void SetAlpha(float alpha)
		{
			if (_AlphaPosition != -1)
				SetUniform(_AlphaPosition, alpha);
		}

		void SetMatrix(const glm::mat4 &matrix)
		{
			if (_WVPPosition != -1)
				SetUniform(_WVPPosition, matrix);
		}

	private:
		int _InTexturePosition;
		int _AlphaPosition;
		int _WVPPosition;
	};
	typedef std::shared_ptr<CommonShaderProgram> CommonShaderProgramPtr;
}

#endif
