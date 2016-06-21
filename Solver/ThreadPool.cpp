#include "stdafx.h"
/*#pragma once
#include "ThreadPool.tpp"

namespace Concurrency
{
	///////////////////////////Worker thread class
	template<typename T>
	Worker<T>::Worker(ThreadPool<T>& pool, TaskFunction op)
		: m_pool(pool), m_taskfunc(op)
	{}
	
	template<typename T>
	void 
	Worker<T>::operator()()
	{
		std::function<void()> task;
		while (true)
		{
			{   // acquire lock
				std::unique_lock<std::mutex> lock(m_pool.m_mutex);

				// look for a work item
				while (!m_pool.m_bStop && m_pool.tasks.empty())
				{ // if there are none wait for notification
					m_pool.m_condition.wait(lock);
				}

				if (m_pool.m_bStop) // exit if the pool is stopped
					return;

				// get the task from the queue
				task = m_pool.tasks.front();
				m_pool.tasks.pop_front();

			}   // release lock

				// execute the task
			m_taskfunc(task);
		}
	};

	///////////////////////////Thread pool  class
	template<typename T>
	ThreadPool<T>::ThreadPool(size_t numThreads, TaskFunction op)
	{
		for (size_t i = 0; i < numThreads; i++)
		{
			m_workers.push_back(std::thread(Worker(*this), op));
		}
	}

	template<typename T>
	void 
	ThreadPool<T>::Enqueue(T* t)
	{
		{ // acquire lock
			std::unique_lock<std::mutex> lock(queue_mutex);

			// add the task
			
			tasks.push_back(std::forward<T>(t));
		} // release lock

			// wake up one thread
		m_condition.notify_one();
	}

	template<typename T>
	ThreadPool<T>::~ThreadPool()
	{
		// stop all threads
		m_bStop = true;
		m_condition.notify_all();

		// join them
		for (size_t i = 0; i < m_workers.size(); i++)
		{
			m_workers[i].join();
		}
	}
}*/