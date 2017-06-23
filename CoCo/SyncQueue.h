#pragma once
#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace CoCo
{
	template<typename T>
	class CSyncQueue
	{
	public:
		CSyncQueue(int vMaxSize) : m_MaxQueueSize(vMaxSize), m_NeedStop(false) {}
		~CSyncQueue() {}

		void put(const T& vElem);
		void put(T&& vElem);
		
		void take(std::list<T>& voList);
		void take(T& voTask);

		void stop();

		std::size_t Size() const;
		std::size_t Count() const;

	private:
		template<typename F>
		void __add(F&& vElem)
		{
			std::unique_lock<std::mutex> Locker(m_Mutex); //unique_lock和lock_guard比，可以随时释放。
			m_NotFull.wait(Locker, [this] {return m_NeedStop || __notFull(); }); //满足条件，获取mutex，结束wait，向下执行。不满足条件阻塞线程。

			if (m_NeedStop) return;

			m_Queue.push_back(std::forward<F>(vElem));
			m_NotEmpty.notify_one();
		}

		bool __notFull() const;
		bool __notEmpty() const;
		
		std::list<T> m_Queue;
		std::mutex m_Mutex;
		std::condition_variable m_NotEmpty;
		std::condition_variable m_NotFull;

		unsigned int m_MaxQueueSize;
		bool m_NeedStop;
	};

// 	template<typename F>
// 	void CoCo::CSyncQueue::__add(F&& vElem)
// 	{
// 		std::unique_lock<std::mutex> Locker(m_Mutex); //unique_lock和lock_guard比，可以随时释放。
// 		m_NotFull.wait(Locker, [this] {return m_NeedStop || __notFull(); }); //满足条件，获取mutex，结束wait，向下执行。不满足条件阻塞线程。
// 
// 		if (m_NeedStop) return;
// 
// 		m_Queue.push_back(std::forward<F>(vElem));
// 		m_NotEmpty.notify_one();
// 	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	void CoCo::CSyncQueue<T>::stop()
	{
		{
			std::lock_guard<std::mutex> Locker(m_Mutex);
			m_NeedStop = true;
		}

		m_NotFull.notify_all();
		m_NotEmpty.notify_all();
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	void CoCo::CSyncQueue<T>::take(T& voTask)
	{
		std::unique_lock<std::mutex> Locker(m_Mutex);
		m_NotEmpty.wait(Locker, [this] {return m_NeedStop || __notEmpty(); });
		if (m_NeedStop) return;

		voTask = m_Queue.front();
		m_Queue.pop_front();
		m_NotFull.notify_one();
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	void CoCo::CSyncQueue<T>::take(std::list<T>& voList)
	{
		std::unique_lock<std::mutex> Locker(m_Mutex);
		m_NotEmpty.wait(Locker, [this] {return m_NeedStop || __notEmpty(); });
		
		if (m_NeedStop) return;

		voList = std::move(m_Queue);
		m_NotFull.notify_one();
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	void CoCo::CSyncQueue<T>::put(const T& vElem)
	{
		__add(vElem);
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	void CoCo::CSyncQueue<T>::put(T&& vElem)
	{
		__add(std::forward<T>(vElem));
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	std::size_t CoCo::CSyncQueue<T>::Count() const
	{
		return m_Queue.size();
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	std::size_t CoCo::CSyncQueue<T>::Size() const
	{
		std::lock_guard<std::mutex> Locker(m_Mutex);
		return m_Queue.size();
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	bool CoCo::CSyncQueue<T>::__notEmpty() const
	{
		bool bEmpty = m_Queue.empty();

		if (bEmpty)
			std::cout << "Sync Queue is empty, please wait...async thead's id " << std::this_thread::get_id() << std::endl;
		return !bEmpty;
	}

	//***********************************************************
	//FUNCTION::
	template<typename T>
	bool CoCo::CSyncQueue<T>::__notFull() const
	{
		bool bFull = m_Queue.size() >= m_MaxQueueSize;

		if (bFull)
			std::cout << "Sync Queue is full, please wait." << std::endl;
		return !bFull;
	}
}


