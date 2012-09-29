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

#include <YA3DE/GUI/GUIManager.h>
#include <YA3DE/GUI/Control.h>

#include <exception>

using namespace YA3DE::GUI;

Control::Control()
{
	_handle = GUIManager::Instance()->GetNewHandle();

	_renderer = NULL;
	_foreColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_backColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	_font = GUIManager::Instance()->GetDefaultFont();
	_fontHeight = 18;
	_position = glm::uvec2(0, 0);
	_size = glm::uvec2(0, 0);
	_parent = NULL;
}

Control::~Control()
{
	if (_renderer)
		delete _renderer;
}

IGUIRenderer *Control::GetRenderer()
{
	if (_renderer == NULL)
		_renderer = GUIManager::Instance()->CreateRenderer(this);

	return _renderer;
}

void Control::SetParent(ControlPtr parent)
{
	if (_parent != NULL)
		_parent->RemoveChild(this->shared_from_this());
	else if (parent != NULL)
		GUIManager::Instance()->RemoveRootControl(this->shared_from_this());

	_parent = parent;
}
