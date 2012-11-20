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

#include <YA3DE/Logger.h>
#include <YA3DE/Application.h>
#include <YA3DE/Content/Dispatcher.h>

#define ASYNC_WORKER_COUNT 1

using namespace YADE;

FunctionDispatcher::FunctionDispatcher()
{
}

void FunctionDispatcher::InitializeAsyncWorkers()
{
	_ExitFlag = false;

	LOG("Starting %d async workers", ASYNC_WORKER_COUNT);
	for (int i = 0; i < ASYNC_WORKER_COUNT; i++)
		_Threads.push_back(std::thread(&FunctionDispatcher::_AsyncWorker, this));
}

void FunctionDispatcher::EnqueueAsync(std::function<void()> func)
{
	if (_Threads.size() == 0)
	{
		func();
	}
	else
	{
		_Guard.lock();
		_Queue.push(func);
		_Guard.unlock();
	}
}

void FunctionDispatcher::Shutdown()
{
	_ExitFlag = true;

	LOG("Waiting worker threads to finish");
	for (unsigned int i = 0; i < _Threads.size(); i++)
		_Threads[i].join();
}

void FunctionDispatcher::_AsyncWorker()
{
	sf::Context context(Application::Instance()->Window().getSettings(), Application::Instance()->Window().getSize().x, Application::Instance()->Window().getSize().y);
	context.setActive(true);

	while (_ExitFlag == false)
	{
		_Guard.lock();

		if (_Queue.size() == 0)
		{
			_Guard.unlock();
			std::this_thread::yield();
			continue;
		}

		std::function<void()> f = _Queue.front();
		_Queue.pop();
		_Guard.unlock();

		f();
	}

	context.setActive(false);
}
