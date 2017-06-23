#include "ThreadPool.h"

void testThreadPool()
{
	CoCo::CTinyThreadPool ThreadPool;

	std::thread Thread1([&ThreadPool] {
		for (int i = 0; i < 10; i++)
		{
			auto ThreadId = std::this_thread::get_id();
			ThreadPool.addTask([ThreadId] {
				std::cout << "ͬ���߳�1���߳�ID�ǣ� " << ThreadId << std::endl;
			});
		}
	});

	std::thread Thread2([&ThreadPool] {
		for (int i = 0; i < 10; i++)
		{
			auto ThreadId = std::this_thread::get_id();
			ThreadPool.addTask([ThreadId] {
				std::cout << "ͬ���߳�2���߳�ID�ǣ� " << ThreadId << std::endl;
			});
		}
	});

	std::this_thread::sleep_for(std::chrono::seconds(2));

	Thread1.join();
	Thread2.join();
}

int main()
{
	testThreadPool();
	return 0;
}