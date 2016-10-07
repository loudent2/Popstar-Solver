#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_set>
#include <unordered_map>
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
		void Pop(T& outVal)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			outVal  = m_queue.front();
			m_queue.pop();
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
	template <typename Key, typename T>
	class ConcurrentUnorderd_Map
	{
	public:
		void Insert(Key key, T& item)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				std::pair<Key, T> insertValue(key, item);
				m_queue.insert(insertValue);
			}
		}
		bool Find(Key key, T& item)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				auto it = m_queue.find(key);
				if (m_queue.end() != it)
				{
					item = it->second;
					return true;
				}
				return false;
			}
		}

		size_t Size()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_queue.size();
		}


	protected:
		std::unordered_map<Key, T> m_queue;
		std::mutex m_mutex;
	};
}