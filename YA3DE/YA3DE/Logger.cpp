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

#include <YA3DE/Logger.h>
#include <exception>
#include <iostream>
#include <stdarg.h>

using namespace YA3DE;

Logger *Logger::_instance = NULL;

Logger::Logger()
{
	if (_instance != NULL)
		throw std::exception("YA3DE::Logger must have only one instance.");

	_filename = "ya3de.log";
	
#ifdef _DEBUG
	_useStdout = true;
#else
	_useStdout = false;
#endif

	_instance = this;
}

Logger::~Logger()
{
	if (_file.is_open())
		_file.close();
}

void Logger::Open()
{
	_file.open(_filename, std::fstream::out | std::fstream::trunc);
}

void Logger::Log(char *format, ...)
{
	static char buffer[2048];
	va_list va;
	int size;

	if (!_file.is_open())
		Open();

	va_start(va, format);
	size = vsprintf(buffer, format, va);
	va_end(va);

	if (_useStdout)
		printf("%s", buffer);

	_file.write(buffer, size);
}

void Logger::Log(std::string s)
{
	if (!_file.is_open())
		Open();

	if (_useStdout)
		std::cout << s;

	_file << s;
}
