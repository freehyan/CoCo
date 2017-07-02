#include "Ioc.hpp"
#include "Test.h"

//FUNCTION: detect the memory leak in DEBUG mode
void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 2716;
#endif
}

//***********************************************************
//FUNCTION::
void testSimple()
{
	CoCo::CIoc Ioc;
	Ioc.registerSimple<CBus>("bus");
	Ioc.registerSimple<CCar>("car");

	std::shared_ptr<CBus> bus = Ioc.createProduct<CBus>("bus");
	bus->testV();

	std::shared_ptr<ICar> car = Ioc.createProduct<CCar>("car");
	car->testV();
}

//***********************************************************
//FUNCTION::
void testDepend()
{
	CoCo::CIoc Ioc;

	//配置依赖关系，并传入参数
	Ioc.registerType<CObject, CDerivedA, int, double>("A");
	auto Product1 = Ioc.createProduct<CObject>("A", 1, 2.0);
	Product1->func();

	//配置依赖关系
	Ioc.registerType<CObject, CDerivedB>("B");
	auto Product2 = Ioc.createProduct<CObject>("B");
	Product2->func();
}

int main()
{
	installMemoryLeakDetector();
	testSimple();
	testDepend();

	return 0;
}