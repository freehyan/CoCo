#pragma once
#include "NonCopyable.hpp"

namespace CoCo
{
	template<typename Func, typename... Args>
	class SAspect : CNonCopyable
	{
		SAspect(Func&& vFunc) : m_Func(std::forward(Func)(vFunc)){}


	private:
		Func m_Func; 
	};

}