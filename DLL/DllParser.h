#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <functional>

namespace CoCo
{
	class CDllParser
	{
	public:
		CDllParser();
		~CDllParser();

		bool load(const std::string& vLibraryName);
		bool unLoad();

		//NOTE:使用std::refult_of来泛化，使用统一返回值消除不同返回值导致的差异
		template <typename T, typename... Args>
		typename std::result_of<std::function<T>(Args...)>::type executeFunc(const std::string& vFuncName, Args&&... vArgs)
		{
			auto Func = __getFunction<T>(vFuncName);
			if (Func == nullptr)
			{
				std::string ErrorStr = "Can not find this function " + vFuncName;
				throw std::exception(ErrorStr.c_str());
			}

			return Func(std::forward<Args>(vArgs)...);
		}

	private:
		template<typename T>
		std::function<T> __getFunction(const std::string& vFuncName)
		{
			auto Itr = m_Func.find(vFuncName);
			if (Itr == m_Func.end())
			{
				auto Addr = GetProcAddress(m_hModule, vFuncName.data());
				if (!Addr)
					return nullptr;
 				m_Func.insert(std::make_pair(vFuncName, Addr));
 				Itr = m_Func.find(vFuncName);
			}

			return std::function<T>((T*)(Itr->second));
		}



		HMODULE m_hModule;
		std::map<std::string, FARPROC> m_Func;
	};
}