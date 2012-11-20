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

#ifndef YA3DE_HELPERS_H
#define YA3DE_HELPERS_H

#define DEFPROP_SELF_RO(access, type, name) \
access: \
	type name()

#define DEFPROP_SELF_RO_C(access, type, name) \
access: \
	const type name()

#define DEFPROP_SELF_RO_R(access, type, name) \
access: \
	type &name()

#define DEFPROP_SELF_RO_RC(access, type, name) \
access: \
	const type &name()

#define DEFPROP_SELF_RW(access, type, name) \
access: \
	void name(const type &value)

#define DEFPROP_RW(access, type, name) \
access: \
	type name() { return _##name; } \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_RW_R(access, type, name) \
access: \
	type &name() { return _##name; } \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_RW_RC(access, type, name) \
access: \
	const type &name() const { return _##name; } \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_RW_P(access, type, name) \
access: \
	type *name() const { return _##name; } \
	void name(type *value) {  _##name = value; } \
private: \
	type *_##name

#define DEFPROP_RO(access, type, name) \
access: \
	type name() { return _##name; } \
private: \
	void name(const type &value) {  _##name = value; } \
	type _##name

#define DEFPROP_RO_R(access, type, name) \
access: \
	type &name() { return _##name; } \
private: \
	void name(const type &value) {  _##name = value; } \
	type _##name

#define DEFPROP_RO_RC(access, type, name) \
access: \
	const type &name() const { return _##name; } \
private: \
	void name(const type &value) {  _##name = value; } \
	type _##name

#define DEFPROP_RO_C(access, type, name) \
access: \
	type name() const { return _##name; } \
private: \
	void name(const type &value) {  _##name = value; } \
	type _##name


#define DEFPROP_RO_P(access, type, name) \
access: \
	type *name() const { return _##name; } \
private: \
	void name(type *value) {  _##name = value; } \
	type *_##name

#define DEFPROP_P_RO(access, type, name) \
access: \
	type name() { return _##name; } \
protected: \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_P_RO_R(access, type, name) \
access: \
	type &name() { return _##name; } \
protected: \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_P_RO_NR(access, type, name) \
access: \
	type &name() { return _##name; } \
protected: \
	type _##name

#define DEFPROP_P_RO_RC(access, type, name) \
access: \
	const type &name() const { return _##name; } \
protected: \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_P_RO_C(access, type, name) \
access: \
	type name() const { return _##name; } \
protected: \
	void name(const type &value) {  _##name = value; } \
private: \
	type _##name

#define DEFPROP_P_RO_P(access, type, name) \
access: \
	type *name() const { return _##name; } \
protected: \
	void name(type *value) {  _##name = value; } \
private: \
	type *_##name

#endif
