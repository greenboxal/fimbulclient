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

#ifndef _CONTENTMANAGER_H_
#define _CONTENTMANAGER_H_

#include <memory>
#include <string>
#include <list>
#include <unordered_map>
#include <stdarg.h>

#include <YA3DE/Content/ContentName.h>
#include <YA3DE/FileSystem/IFileSystem.h>
#include <YA3DE/FileSystem/IFile.h>
#include <YA3DE/FileSystem/FolderFileSystem.h>

namespace YA3DE
{
	namespace Content
	{
		class Resource : std::enable_shared_from_this<Resource>
		{
		public:
			virtual ~Resource() { }
		};

		class ContentManager
		{
		public:
			ContentManager()
			{
				if (_instance != NULL)
					throw std::exception("YA3DE::FileSystem::FileManager must have only one instance.");

				_instance = this;
			}

			template<class T>
			std::shared_ptr<T> Load(const std::string &name)
			{
				CacheMap::iterator it = _cache.find(name);

				if (it != _cache.end())
				{
					if (!it->second.expired())
						return std::static_pointer_cast<T, Resource>(it->second.lock());
					else
						_cache.erase(it);
				}

				return LoadNew<T>(name);
			}
			
			template<class T>
			void CacheObject(const std::string &name, const std::shared_ptr<T> &ptr)
			{
				_cache[name] = std::weak_ptr<Resource>(ptr);
			}

			template<class T>
			std::shared_ptr<T> LoadNew(const std::string &name);

			static ContentManager *Instance()
			{
				if (_instance == NULL)
					_instance = new ContentManager();

				return _instance;
			}

		private:
			typedef ContentNameTraits<std::weak_ptr<Resource> >::HashTable CacheMap;

			CacheMap _cache;
			static ContentManager *_instance;
		};
	}
}

#endif
