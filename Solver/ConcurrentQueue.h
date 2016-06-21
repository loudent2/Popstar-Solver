#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include  <unordered_set>
namespace PopStarSolver
{
	template <class T>
	class ConcurrentQueue
	{
	public:
		void Push(T& item)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(item);
		}
		T& Pop()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			T retVal = m_queue.front();
			m_queue.pop();
			return retVal;
		}
		size_t Size()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_queue.size();
		}
	protected:
		std::queue<T> m_queue;
		std::mutex m_mutex;
	};

	template <class T>
	class ConcurrentUnorderd_Set
	{
	public:
		void Insert(T& item)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_queue.insert(item);
			}
		}
		bool Find(T& item)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (m_queue.end() != m_queue.find(item))
				{
					return true;
				}
				return false;
			}
		}

	protected:
		std::unordered_set<T> m_queue;
		std::mutex m_mutex;

	};
}