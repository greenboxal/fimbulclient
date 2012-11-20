/*	This file is part of FimbulwinterClient.

	FimbulwinterClient is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	FimbulwinterClient is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FimbulwinterClient.  If not, see <http://www.gnu.org/licenses/>. */

#include <fclient/RagnarokClient.h>

#include <YA3DE/Logger.h>
#include <YA3DE/OpenGL.h>

#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

#include <boost/property_tree/xml_parser.hpp>

using namespace fclient;
using namespace YADE;
using namespace Awesomium;

RagnarokClient::RagnarokClient()
{
	
}

RagnarokClient::~RagnarokClient()
{

}

void RagnarokClient::Initialize()
{
	LOG("Reading configuration file: ragnarok.xml");
	boost::property_tree::read_xml("ragnarok.xml", _Settings);
}

void RagnarokClient::Load()
{
	_Gui = new GuiManager(_Window);
	_Gui->Load();
	_Gui->SetDesktop("login");
}

void RagnarokClient::OnEvent(const sf::Event &e, double elapsed)
{
	_Gui->DispatchEvent(e);
}

void RagnarokClient::Update(double elapsed)
{
	_Gui->Update();
}

void RagnarokClient::Render(double elapsed)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_Gui->Render();
}

void RagnarokClient::Unload()
{
	_Gui->Unload();
}
