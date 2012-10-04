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

#include <YA3DE/OpenGL.h>
#include <YA3DE/Graphics/Shader.h>

using namespace YA3DE;
using namespace YA3DE::Graphics;

Shader::Shader(int type)
{
	_Type = type;
	_ID = glCreateShader(type);
}

Shader::~Shader()
{
	glDeleteShader(_ID);
}

bool Shader::Load(std::string source)
{
	const char *src = source.c_str();
	const char *sources[2];

	if (_Type == GL_VERTEX_SHADER)
		sources[0] = "#define VERTEX_SHADER\n";
	else if (_Type == GL_FRAGMENT_SHADER)
		sources[0] = "#define FRAGMENT_SHADER\n";
	else if (_Type == GL_GEOMETRY_SHADER)
		sources[0] = "#define GEOMETRY_SHADER\n";
	else
		sources[0] = "";

	sources[1] = src;

	glShaderSource(_ID, 2, sources, NULL);
	glCompileShader(_ID);

	int compiled;
	glGetShaderiv(_ID, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		int len;
		glGetShaderiv(_ID, GL_INFO_LOG_LENGTH, &len);
 
		char *log = new char[len];
 
		glGetShaderInfoLog(_ID, len, &len, log);
		_ErrorMessage = std::string(log);

		delete[] log;
	}

	return compiled == GL_TRUE;
}
