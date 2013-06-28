/*
 * PTBE Library
 * Copyright (c) 2013 O01eg <o01eg@yandex.ru>
 * Code covered by the MIT License
 */

#include "vm/environment.h"

#include "thread-pool.h"

void ThreadPool::Thread::operator()()
{
	std::function<void(VM::Environment&)> task;
	VM::Environment env;
	while(true)
	{
		{
			std::unique_lock<std::mutex> lock(m_ThreadPool.m_TasksMutex);

			m_ThreadPool.m_ThreadsWaiting ++;
			m_ThreadPool.m_Notifier.notify_all();

			while((! m_ThreadPool.m_Finish) && (m_ThreadPool.m_Tasks.empty()))
			{
				m_ThreadPool.m_Notifier.wait(lock);
			}

			if(m_ThreadPool.m_Finish)
			{
				return;
			}

			task = std::move(m_ThreadPool.m_Tasks.front());
			m_ThreadPool.m_Tasks.pop();

			m_ThreadPool.m_ThreadsWaiting --;
		}

		task(env);
	}
}

ThreadPool::ThreadPool(size_t init_size)
: m_Finish(false)
, m_ThreadsWaiting(0)
{
	for(size_t i = 0; i < init_size; ++ i)
	{
		m_Pool.push_back(std::thread(ThreadPool::Thread(*this)));
	}
}

ThreadPool::~ThreadPool()
{
	Stop();
	Wait();
}

void ThreadPool::Wait()
{
	for(auto& thread : m_Pool)
	{
		// don't join un-joinable threads.
		if(thread.joinable())
		{
			thread.join();
		}
	}
}

void ThreadPool::Stop()
{
	m_Finish = true;
	m_Notifier.notify_all();	
}

