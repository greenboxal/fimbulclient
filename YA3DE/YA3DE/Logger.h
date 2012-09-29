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

#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <fstream>

#define LogError(format, ...) Logger::Instance()->Log("[Error]: "); Logger::Instance()->Log(format, __VA_ARGS__)
#define LogWarning(format, ...) Logger::Instance()->Log("[Warning]: "); Logger::Instance()->Log(format, __VA_ARGS__)
#define LogInfo(format, ...) Logger::Instance()->Log("[Info]: "); Logger::Instance()->Log(format, __VA_ARGS__)

namespace YA3DE
{
	class Logger
	{
	public:
		Logger();
		~Logger();

		void Log(char *format, ...);
		void Log(std::string s);

		static Logger *Instance()
		{
			if (_instance == NULL)
				_instance = new Logger();

			return _instance;
		}
	private:
		static Logger *_instance;

		void Open();

		bool _useStdout;
		std::string _filename;
		std::fstream _file;
	};
}

#endif
