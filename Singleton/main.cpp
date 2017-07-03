#include "Singleton.h"
#include <memory>
#include <iostream>
#include <string>

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

class CTestA
{
public:
	CTestA(const std::string& vStr) { std::cout << "lvalue" << std::endl; }
	CTestA(std::string&& vStr) { std::cout << "rvalue" << std::endl; }
};

class CTestB
{
public:
	CTestB(const std::string& vStr) { std::cout << "lvalue" << std::endl; }
	CTestB(std::string&& vStr) { std::cout << "rvalue" << std::endl; }
};

class CTestC
{
public:
	CTestC(int vX, double vY) { }
	void func() { std::cout << "test" << std::endl; }
};

int main()
{
	installMemoryLeakDetector();

	std::string InputStr = "test";
	CoCo::CSingleton<CTestA>::Instance(InputStr);
	CoCo::CSingleton<CTestA>::Instance(InputStr);
	CoCo::CSingleton<CTestB>::Instance(std::move(InputStr));

	CoCo::CSingleton<CTestC>::Instance(1, 2.2);
	CoCo::CSingleton<CTestC>::getInstance()->func();

	CoCo::CSingleton<CTestA>::destroyInstance();
	CoCo::CSingleton<CTestB>::destroyInstance();
	CoCo::CSingleton<CTestC>::destroyInstance();

	return 0;
}