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

#ifndef _HELPERS_H_
#define _HELPERS_H_

#ifdef _MSC_VER
#define STRUCT_PACKED
#else
#define STRUCT_PACKED __attribute__((packed))
#endif

#define DEFPROP_SELF_RO(access, type, name) \
access: \
	const type &name()

#define DEFPROP_SELF_RW(access, type, name) \
access: \
	DEFPROP_I_RO_R(access, type, name, Get##name) \
	type &Get##name()

#define DEFPROP_RW(access, type, name) \
access: \
	type &name() { return _##name; } \
private: \
	type _##name

#define DEFPROP_RO(access, type, name) \
access: \
	const type &name() { return _##name; } \
private: \
	type _##name

#endif
