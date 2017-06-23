#include "ThreadPool.h"

using namespace CoCo;

CoCo::CTinyThreadPool::CTinyThreadPool(int vThreadNums /*= std::thread::hardware_concurrency()*/) : m_TaskQueue(MAX_TASK_COUNT)
{
	__startTask(vThreadNums);
}

CoCo::CTinyThreadPool::~CTinyThreadPool()
{
	stop();
}

//***********************************************************
//FUNCTION::
void CoCo::CTinyThreadPool::addTask(const ITask& vTask)
{
	m_TaskQueue.put(vTask);
}

//***********************************************************
//FUNCTION::
void CoCo::CTinyThreadPool::stop()
{
	std::call_once(m_Flag, [this] {__stopThreadGroup();});
}

//***********************************************************
//FUNCTION::
void CoCo::CTinyThreadPool::addTask(ITask&& vTask)
{
	m_TaskQueue.put(std::forward<ITask>(vTask));
}

//***********************************************************
//FUNCTION::
void CoCo::CTinyThreadPool::__startTask(int vThreadNums)
{
	m_Running = true;

	for (int i=0; i<vThreadNums; i++)
	{
		m_ThreadGroup.push_back(std::make_shared<std::thread>(&CTinyThreadPool::__runThreadTask, this));
	}
}

//***********************************************************
//FUNCTION::
void CoCo::CTinyThreadPool::__runThreadTask()
{
	while (m_Running)
	{
		std::list<ITask> TaskList;
		m_TaskQueue.take(TaskList);

		for (auto& task : TaskList)
		{
			if (!m_Running) return;

			task();
		}
	}
}

//***********************************************************
//FUNCTION::
void CoCo::CTinyThreadPool::__stopThreadGroup()
{
	m_TaskQueue.stop();
	m_Running = false;

	for (auto thread : m_ThreadGroup)
	{
		if (thread)
			thread->join();
	}

	m_ThreadGroup.clear();
}