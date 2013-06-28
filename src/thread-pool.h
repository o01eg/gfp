/*
 * PTBE Library
 * Copyright (c) 2013 O01eg <o01eg@yandex.ru>
 * Code covered by the MIT License
 */

#ifndef _PTBE_THREAD_POOL_HPP_
#define _PTBE_THREAD_POOL_HPP_

#include <queue>
#include <thread>
#include <vector>

/// \brief Managing of thread. Base on http://progsch.net/wordpress/?p=81
/// \todo Add busy checking.
class ThreadPool
{
	/// \brief Class for executing tasks.
	class Thread
	{
	public:
		/// \brief Constructor. Manage tasks in threads.
		/// \param[in] pool Thread pool.
		Thread(ThreadPool &pool)
		: m_ThreadPool(pool)
		{
		}

		/// \brief Thread code.
		/// \return Error status.
		void operator()();
	private:
		ThreadPool &m_ThreadPool; ///< Thread Pool.
	};
public:
	/// \brief Constructor. Create threads.
	/// \param[in] init_size Initial count of threads.
	ThreadPool(size_t init_size);

	/// \brief Destructor.
	~ThreadPool();

	/// \brief Add task for worker.
	/// \param[in] task Task.
	void AddTask(std::function<void(VM::Environment&)> &&task)
	{
		{
			std::unique_lock<std::mutex> lock(m_TasksMutex);
			m_Tasks.push(task);
		}
		m_Notifier.notify_one();
	}

	/// \brief Check if deque empty.
	/// \return Is empty?
	bool IsEmpty() const
	{
		return m_Tasks.empty();
	}

	void WaitFinish() const
	{
		std::unique_lock<std::mutex> lock(m_TasksMutex);
		m_Notifier.wait(lock, [this](){ return m_ThreadsWaiting == m_Pool.size(); });
	}

	/// \brief Wait for threads finished.
	void Wait();

	/// \brief Stop thread pool.
	void Stop();
private:
	std::vector<std::thread> m_Pool; ///< Array of threads.
	std::queue<std::function<void(VM::Environment&)>> m_Tasks; ///< Deque of tasks.

	mutable std::mutex m_TasksMutex; ///< Lock deque access.
	mutable std::condition_variable m_Notifier; ///< Notify threads.

	volatile bool m_Finish; ///< Mark closing pool.
	size_t m_ThreadsWaiting;
};

#endif // _PTBE_THREAD_POOL_HPP_

