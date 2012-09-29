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

#ifndef _GUILABEL_H_
#define _GUILABEL_H_

#include <memory>
#include <list>
#include <YA3DE/Math.h>
#include <YA3DE/GUI/Control.h>

namespace YA3DE
{
	namespace GUI
	{
		class Label : public Control
		{
		public:
			Label()
			{

			}

			virtual ~Label()
			{

			}

			virtual std::string GetClass()
			{
				return "YA3DE::GUI::Label";
			}
		};
		typedef std::shared_ptr<Label> LabelPtr;
	}
}

#endif
