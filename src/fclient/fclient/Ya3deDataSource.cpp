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

#include <fclient/Ya3deDataSource.h>
#include <YA3DE/FileSystem/FileManager.h>

#include <Awesomium/STLHelpers.h>

using namespace fclient;
using namespace YADE;
using namespace Awesomium;

YadeDataSource::YadeDataSource()
{
}

YadeDataSource::~YadeDataSource()
{

}

void YadeDataSource::OnRequest(int request_id, const Awesomium::WebString &path)
{
	std::string spath = ToString(path);
	FilePtr file = FileManager::Instance()->OpenFile("data/" + spath);

	if (file)
	{
		size_t len = file->GetSize();
		char *data = new char[file->GetSize()];
		file->Read(data, len);
		file->Close();

		SendResponse(request_id, len, (unsigned char *)data, MimeFromExtension(spath));
	}
	else
	{
		SendResponse(request_id, 0, nullptr, WSLit("404"));
	}
}

Awesomium::WebString YadeDataSource::MimeFromExtension(const std::string &path)
{
	std::string ext = path.substr(path.find_last_of(".") + 1);
	const char *mime = "text/html";

	if (ext == "html") mime = "text/html";
	else if (ext == "htm") mime = "text/html";
	else if (ext == "css") mime = "text/css";
	else if (ext == "less") mime = "text/less";
	else if (ext == "js") mime = "text/javascript";
	else if (ext == "jpg") mime = "image/jpeg";
	else if (ext == "gif") mime = "image/gif";
	else if (ext == "png") mime = "image/png";

	return WSLit(mime);
}
