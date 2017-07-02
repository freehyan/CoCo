#pragma once
#include <list>
#include <memory>
#include <thread>
#include <functional>
#include <atomic>
#include "SyncQueue.h"

#define MAX_TASK_COUNT 100

namespace CoCo
{
	class CTinyThreadPool
	{
	public:
		using ITask = std::function<void()>;

		CTinyThreadPool(int vThreadNums = std::thread::hardware_concurrency());
		~CTinyThreadPool();

		void addTask(const ITask& vTask);
		void addTask(ITask&& vTask);

		void stop();

	private:
		void __startTask(int vThreadNums);
		void __runThreadTask();
		void __stopThreadGroup();

		std::list<std::shared_ptr<std::thread>> m_ThreadGroup;
		CSyncQueue<ITask> m_TaskQueue;

		std::atomic_bool m_Running;
		std::once_flag m_Flag;
	};
}