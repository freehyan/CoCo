#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CoCo/ThreadPool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	private:
		CoCo::CTinyThreadPool m_ThreadPool;
		int m_Res;

	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			Logger::WriteMessage("Test initialized.\n");
		}

		TEST_METHOD_CLEANUP(TearDown)
		{
			Logger::WriteMessage("Test completed.\n");
		}


		TEST_METHOD(TestMethod1)
		{
			std::thread Thread1([this] {
				for (int i = 0; i < 10; i++)
				{
					auto ThreadId = std::this_thread::get_id();
					m_ThreadPool.addTask(
						[this] {m_Res += 10; }
					);
				}
			});

			Thread1.join();
			Assert::AreEqual<int>(m_Res, 100);
			Logger::WriteMessage("Thread pool tested.\n");
		}

	};
}