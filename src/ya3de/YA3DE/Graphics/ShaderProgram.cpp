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

#include <YA3DE/Math.h>
#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>
#include <YA3DE/Content/ContentManager.h>
#include <YA3DE/FileSystem/FileManager.h>
#include <YA3DE/Graphics/ShaderProgram.h>

#include <glm/gtc/type_ptr.hpp>

using namespace YADE;

ShaderProgram::ShaderProgram()
{
	_ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	std::list<ShaderPtr>::iterator it;
	for (it = _Shaders.begin(); it != _Shaders.end(); it++)
		glDetachShader(_ID, (*it)->ID());

	glDeleteProgram(_ID);
}

void ShaderProgram::AddShader(ShaderPtr shader)
{
	_Shaders.push_front(shader);
	glAttachShader(_ID, shader->ID());
}

bool ShaderProgram::Link()
{
	glLinkProgram(_ID);

	int compiled;
	glGetProgramiv(_ID, GL_LINK_STATUS, &compiled);

	return compiled == 1;
}

void ShaderProgram::Begin()
{
	glUseProgram(_ID);
}

int ShaderProgram::GetUniformID(const std::string &name)
{
	return glGetUniformLocation(_ID, name.c_str());
}

void ShaderProgram::End()
{
	glUseProgram(0);
}

template<>
void ShaderProgram::SetUniform(int id, const glm::mat3 &value)
{
	glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::dmat3 &value)
{
	glUniformMatrix3dv(id, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::mat4 &value)
{
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::dmat4 &value)
{
	glUniformMatrix4dv(id, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const float &value)
{
	glUniform1f(id, value);
}

template<>
void ShaderProgram::SetUniform(int id, const double &value)
{
	glUniform1d(id, value);
}

template<>
void ShaderProgram::SetUniform(int id, const int &value)
{
	glUniform1i(id, value);
}

template<>
void ShaderProgram::SetUniform(int id, const glm::vec2 &value)
{
	glUniform2fv(id, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::dvec2 &value)
{
	glUniform2dv(id, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::uvec2 &value)
{
	glUniform2iv(id, 1, (GLint *)glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::vec3 &value)
{
	glUniform3fv(id, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::dvec3 &value)
{
	glUniform3dv(id, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::uvec3 &value)
{
	glUniform3iv(id, 1, (GLint *)glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::vec4 &value)
{
	glUniform4fv(id, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::dvec4 &value)
{
	glUniform4dv(id, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::SetUniform(int id, const glm::uvec4 &value)
{
	glUniform4iv(id, 1, (GLint *)glm::value_ptr(value));
}

template<>
std::shared_ptr<ShaderProgram> ContentManager::LoadNew(const std::string &name, bool async)
{
	FilePtr fp = FileManager::Instance()->OpenFile(name);

	if (!fp)
	{
		LOG("Failed loading '%s': file not found", name.c_str());
		return nullptr;
	}

	int size = fp->GetSize();
	char *data = new char[size + 1];
	fp->Read(data, size);
	fp->Close();

	data[size] = 0;

	ShaderProgramPtr program = std::make_shared<ShaderProgram>();

	ShaderPtr vs = std::make_shared<Shader>(GL_VERTEX_SHADER);
	if (!vs->Load(data))
	{
		LOG("Error loading '%s' vertex shader: %s", name.c_str(), vs->ErrorMessage().c_str());
		return nullptr;
	}
	program->AddShader(vs);
	
	ShaderPtr ps = std::make_shared<Shader>(GL_FRAGMENT_SHADER);
	if (!ps->Load(data))
	{
		LOG("Error loading '%s' fragment shader: %s", name.c_str(), vs->ErrorMessage().c_str());
		return nullptr;
	}
	program->AddShader(ps);

	if (!program->Link())
	{
		LOG("Error loading '%s': link failed", name.c_str());
		return nullptr;
	}

	ContentManager::Instance()->CacheObject(name, program);
	
	LOG("Loaded shader '%s'", name.c_str());

	return program;
}
