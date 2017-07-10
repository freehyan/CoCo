
//建立dll工程导出减法函数
// #ifdef __cplusplus  
// extern "C" {
// #endif
// 
// 	int __declspec(dllexport) subVal(int a, int b) { return a - b; }
// 
// #ifdef __cplusplus
// }
// #endif

//传统方式显示调用DLL，繁琐
//#include <iostream>
//#include <Windows.h>
//
//void testDll()
//{
//	typedef int(*pMaxFunc)(int, int);
//	typedef int(*pGetFunc)(int);
//
//	HINSTANCE hInstLibrary = LoadLibrary(L"MyDLL.dll");
//	if (hInstLibrary == nullptr)
//		return;
//
//	pMaxFunc pMax = (pMaxFunc)GetProcAddress(hInstLibrary, "subVal");
//	if (pMax == nullptr)
//		return;
//	int ret = pMax(5, 8);
//
//	pGetFunc pGet = (pGetFunc)GetProcAddress(hInstLibrary, "Get");
//	if (pGet == nullptr)
//		return;
//	int ret2 = pGet(5);
//
//	FreeLibrary(hInstLibrary);
//}

#include "DllParser.h"

void testDll()
{
	CoCo:: CDllParser Dllparser;
	Dllparser.load("MyDLL.dll");
	auto Res = Dllparser.executeFunc<int(int, int)>("subVal", 3, 5);
}

int main()
{
	testDll();

	return 0;
}