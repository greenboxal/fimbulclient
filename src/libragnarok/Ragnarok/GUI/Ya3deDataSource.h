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

#ifndef FCLIENT_YA3DEDATASOURCE_H
#define FCLIENT_YA3DEDATASOURCE_H

#include <string>
#include <Awesomium/DataSource.h>

namespace Ragnarok
{
	class YadeDataSource : public Awesomium::DataSource
	{
	public:
		YadeDataSource();
		virtual ~YadeDataSource();

		virtual void OnRequest(int request_id, const Awesomium::WebString &path);

	private:
		Awesomium::WebString MimeFromExtension(const std::string &path);
	};
}

#endif
