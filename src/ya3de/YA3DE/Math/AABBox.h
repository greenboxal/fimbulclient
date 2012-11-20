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

#ifndef YA3DE_AABBOX_H
#define YA3DE_AABBOX_H

#include <algorithm>

#include <YA3DE/Math.h>
#include <YA3DE/Helpers.h>

namespace YADE
{
	class AABBox
	{
	public:
		AABBox(const glm::vec3 &min, const glm::vec3 &max)
			: _Min(min), _Max(max)
		{

		}

		AABBox &operator+(const glm::vec3 &vertex)
		{
			_Min.x = std::min(_Min.x, vertex.x);
			_Min.y = std::min(_Min.y, vertex.y);
			_Min.z = std::min(_Min.z, vertex.z);

			_Max.x = std::max(_Max.x, vertex.x);
			_Max.y = std::max(_Max.y, vertex.y);
			_Max.z = std::max(_Max.z, vertex.z);

			return (*this);
		}

		AABBox &operator+(const AABBox &other)
		{
			_Min.x = std::min(_Min.x, other._Min.x);
			_Min.y = std::min(_Min.y, other._Min.y);
			_Min.z = std::min(_Min.z, other._Min.z);

			_Max.x = std::max(_Max.x, other._Max.x);
			_Max.y = std::max(_Max.y, other._Max.y);
			_Max.z = std::max(_Max.z, other._Max.z);

			return (*this);
		}

		AABBox &operator+=(const glm::vec3 &vertex)
		{
			_Min.x = std::min(_Min.x, vertex.x);
			_Min.y = std::min(_Min.y, vertex.y);
			_Min.z = std::min(_Min.z, vertex.z);

			_Max.x = std::max(_Max.x, vertex.x);
			_Max.y = std::max(_Max.y, vertex.y);
			_Max.z = std::max(_Max.z, vertex.z);

			return (*this);
		}

		AABBox &operator+=(const AABBox &other)
		{
			_Min.x = std::min(_Min.x, other._Min.x);
			_Min.y = std::min(_Min.y, other._Min.y);
			_Min.z = std::min(_Min.z, other._Min.z);

			_Max.x = std::max(_Max.x, other._Max.x);
			_Max.y = std::max(_Max.y, other._Max.y);
			_Max.z = std::max(_Max.z, other._Max.z);

			return (*this);
		}

		void CalculateRangeAndOffset()
		{
			_Range = (_Max + _Min) / 2.0F;
			_Offset = (_Max - _Min) / 2.0F;
		}

		DEFPROP_RO(public, glm::vec3, Min);
		DEFPROP_RO(public, glm::vec3, Max);
		DEFPROP_RO(public, glm::vec3, Range);
		DEFPROP_RO(public, glm::vec3, Offset);
	};
}

#endif
