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
#include <YA3DE/Content/Dispatcher.h>

#define ASYNC_WORKER_COUNT 0

using namespace YADE;

FunctionDispatcher::FunctionDispatcher()
{
}

void FunctionDispatcher::InitializeAsyncWorkers()
{
}

void FunctionDispatcher::EnqueueAsync(std::function<void()> func)
{
}

void FunctionDispatcher::Shutdown()
{
}
/*
void FunctionDispatcher::_AsyncWorker(YA3DE::System::GLContext *context)
{
	context->MakeCurrent();
	context->InitGlew();

	while (_ExitFlag == false)
	{
		_AsyncGuard.lock();

		if (_AsyncQueue.size() == 0)
		{
			_AsyncGuard.unlock();
			std::this_thread::yield();
			continue;
		}

		std::function<void()> f = _AsyncQueue.front();
		_AsyncQueue.pop();
		_AsyncGuard.unlock();

		f();
	}

	context->UnbindCurrent();
}
*/
