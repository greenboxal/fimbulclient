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

#include <YA3DE/Logger.h>

#include <exception>
#include <iostream>

#include <stdarg.h>
#include <time.h>

using namespace YA3DE;

Logger *Logger::_Instance = NULL;

Logger::Logger()
{
	if (_Instance != NULL)
		throw std::exception("YA3DE::Logger must have only one instance.");

	_Filename = "ya3de.log";
	
#ifdef _DEBUG
	_UseStdout = true;
#else
	_UseStdout = false;
#endif

	_Instance = this;
}

Logger::~Logger()
{
	if (_File.is_open())
		_File.close();
}

void Logger::Open()
{
	_File.open(_Filename, std::fstream::out | std::fstream::trunc);
}

void Logger::Log(char *format, ...)
{
	static char buffer[50 * 1024];
	va_list va;
	int size;
	
	time_t rawtime;
	struct tm *timeinfo;
	char timestr[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	int timestrsize = strftime(timestr, sizeof(timestr), "%X: ", timeinfo);

	if (!_File.is_open())
		Open();

	va_start(va, format);
	size = vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	_Mutex.lock();
	if (_UseStdout)
	{
		printf("%s", timestr);
		printf("%s\n", buffer);
	}

	_File.write(timestr, timestrsize);
	_File.write(buffer, strlen(buffer));
	_File.write("\n", 1);
	_Mutex.unlock();
}

void Logger::Log(std::string s)
{
	time_t rawtime;
	struct tm *timeinfo;
	char timestr[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(timestr, sizeof(timestr), "%X: ", timeinfo);

	if (!_File.is_open())
		Open();
	
	_Mutex.lock();
	if (_UseStdout)
		std::cout << s;
	
	_File << timestr;
	_File << s;
	_File << "\n";
	_Mutex.unlock();
}
