#include "Observer.h"
#include <memory>

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
	void print(int vA, int vB)
	{ 
		std::cout << vA << " " << vB << std::endl;
	}
};

void print(int vA, int vB) 
{ 
	std::cout << vA << " " << vB << std::endl;
}


int main()
{
	installMemoryLeakDetector();

	CoCo::CObserver<std::function<void(int, int)>> Observer;
	auto ObserverId = Observer.attach(print);

	CTestA TestA;
	auto LambdaKey = Observer.attach([&TestA](int a, int b) { std::cout << a << " " << b << std::endl; });

	//std::function
	std::function<void(int, int)> Func = std::bind(&CTestA::print, &TestA, std::placeholders::_1, std::placeholders::_2);
	auto FuncId = Observer.attach(Func);

	int A = 1, B = 2;
	Observer.notify(A, B);
	Observer.detach(ObserverId);

	return 0;
}