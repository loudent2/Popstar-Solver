#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <deque>
#include <stack>

namespace Concurrency
{
	//forward declaration
	template<typename T>
	class ThreadPool;

	template<typename T>
	class Worker
	{
		using TaskFunction = std::function<void(T*, ThreadPool<T>&)>;
	public:
		Worker(ThreadPool<T>& pool, TaskFunction op);
		void operator()();		

	protected:
		ThreadPool<T>& m_pool;
		TaskFunction m_taskfunc;
	};

	
	template<typename T>
	class ThreadPool
	{
		using TaskFunction = std::function<void(T*, ThreadPool<T>&)>;
	public:
		//Unimplmement default copy and move constructors;
		ThreadPool() = delete;
		ThreadPool(ThreadPool& other) = delete;
		ThreadPool(ThreadPool&& other) = delete;
		
		//constructor
		ThreadPool(size_t numThreads, TaskFunction op);
		~ThreadPool();

		//add tasks to queue
		void Enqueue(T* t);

				//allows the caller to wait until the work is done otherwise
		//it will run forever
		std::condition_variable workIsDone;

	protected:
		friend class Worker<T>;
		std::vector<std::thread> m_workers;

		// the task queue
		std::stack<T*> tasks;

		std::mutex m_mutex;
		std::condition_variable m_condition;
		std::atomic<bool> m_bStop = {false};

	};
}