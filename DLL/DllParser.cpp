#include "DllParser.h"
#include <iostream>
using namespace CoCo;

CDllParser::CDllParser() : m_hModule(nullptr)
{
}

CDllParser::~CDllParser()
{
	unLoad();
}

//***********************************************************
//FUNCTION::
bool CoCo::CDllParser::load(const std::string& vLibraryName)
{
	m_hModule = LoadLibraryA(vLibraryName.data());
	if (nullptr == m_hModule)
	{
		std::cout << "Loadlibrary failed." << std::endl;
		return false;
	}

	return true;
}

//***********************************************************
//FUNCTION::
bool CoCo::CDllParser::unLoad()
{
	if (m_hModule == nullptr)
		return true;
	auto b = FreeLibrary(m_hModule);
	if (!b)
		return false;
	m_hModule = nullptr;
	return true;
}
