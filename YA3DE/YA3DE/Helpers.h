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

#ifdef _MSC_VER
#define STRUCT_PACKED
#else
#define STRUCT_PACKED __attribute__((packed))
#endif

#if defined(_MSC_VER)

#define DEFPROP_I_RW(access, type, name, getter, setter) \
access: \
	__declspec(property(get = getter, put = setter)) type name

#define DEFPROP_I_RW_R(access, type, name, getter, setter) \
access: \
	__declspec(property(get = getter, put = setter)) type &name

#define DEFPROP_I_RO(access, type, name, getter) \
access: \
	__declspec(property(get = getter)) type name

#define DEFPROP_I_RO_R(access, type, name, getter) \
access: \
	__declspec(property(get = getter)) type &name

#define DEFPROP_SELF_RO(access, type, name) \
access: \
	DEFPROP_I_RO(access, type, name, Get##name); \
	type Get##name()

#define DEFPROP_SELF_RO_R(access, type, name) \
access: \
	DEFPROP_I_RO_R(access, type, name, Get##name) \
	type &Get##name()

#define DEFPROP_RW(access, type, name) \
access: \
	DEFPROP_I_RW(access, type, name, Get##name, Set##name); \
	type Get##name() const { return _##name; } \
	void Set##name(const type &value) { _##name = value; } \
private: \
	type _##name

#define DEFPROP_RW_P(access, type, name) \
access: \
	DEFPROP_I_RW(access, type, name, Get##name, Set##name); \
	type Get##name() const { return _##name; } \
	void Set##name(type value) { _##name = value; } \
private: \
	type _##name

#define DEFPROP_RW_R(access, type, name) \
access: \
	DEFPROP_I_RW_R(access, type, name, Get##name, Set##name); \
	type &Get##name() { return _##name; } \
	void Set##name(const type &value) { _##name = value; } \
private: \
	type _##name

#define DEFPROP_RW_PR(access, type, name) \
access: \
	DEFPROP_I_RW(access, type, name, Get##name, Set##name); \
	type &Get##name() { return _##name; } \
	void Set##name(type value) { _##name = value; } \
private: \
	type _##name

#define DEFPROP_RO(access, type, name) \
access: \
	DEFPROP_I_RO(access, type, name, Get##name); \
	type Get##name() const { return _##name; } \
private: \
	type _##name

#define DEFPROP_RO_R(access, type, name) \
access: \
	DEFPROP_I_RO_R(access, type, name, Get##name); \
	type &Get##name() { return _##name; } \
private: \
	type _##name

#else
#error "This compiler doesn't support property creation!"
#endif

