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

#include <YA3DE/GUI/GUIManager.h>

using namespace YA3DE::GUI;

GUIManager *GUIManager::_instance;

void GUIManager::Update(double elapsedTime)
{
	std::map<int, ControlPtr>::iterator it;

	for (it = _rootWindows.begin(); it != _rootWindows.end(); it++)
		UpdateControl(it->second, elapsedTime);
}

void GUIManager::Render(double elapsedTime)
{
	std::map<int, ControlPtr>::iterator it;

	for (it = _rootWindows.begin(); it != _rootWindows.end(); it++)
		RenderControl(it->second, elapsedTime);
}

void GUIManager::UpdateControl(ControlPtr control, double elapsedTime)
{
	std::list<ControlPtr>::iterator it;
	std::list<ControlPtr> &childs = control->GetChilds();

	control->Update(elapsedTime);

	for (it = childs.begin(); it != childs.end(); it++)
		UpdateControl(*it, elapsedTime);
}

void GUIManager::RenderControl(ControlPtr control, double elapsedTime)
{
	std::list<ControlPtr>::iterator it;
	std::list<ControlPtr> &childs = control->GetChilds();
	Graphics2D &g = CreateGraphics(control);

	g.Begin();
	control->GetRenderer()->Render(g);
	g.End();

	for (it = childs.begin(); it != childs.end(); it++)
		RenderControl(*it, elapsedTime);
}

Graphics2D GUIManager::CreateGraphics(ControlPtr control)
{
	return Graphics2D(GetAbsoluteControlBound(control));
}

glm::uvec4 GUIManager::GetAbsoluteControlBound(ControlPtr ptr)
{
	ControlPtr walk = NULL;
	glm::uvec2 pos(0);

	pos += ptr->GetPosition();

	while((walk = ptr->GetParent()) != NULL)
		pos += walk->GetPosition();

	return glm::uvec4(pos, ptr->GetSize());
}
