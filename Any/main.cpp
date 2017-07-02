#include "Any.hpp"

void testAny()
{
	CoCo::CAny Any;
	bool bRes = Any.isNull();
	std::string TestStr = "hello";
	Any = TestStr;
}

int main()
{
	testAny();

	return 0;
}