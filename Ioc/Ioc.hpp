#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <unordered_map>
#include "NonCopyable.hpp"
#include "Any.hpp"

namespace CoCo
{
	class CIoc : public CNonCopyable
	{
	public:
		CIoc(void){}
		~CIoc(void){}
		
		template<class T, typename Depend, typename... Args>
		void registerType(const std::string& vKey);

		template<class T, typename... Args>
		std::shared_ptr<T> createProduct(const std::string& vKey, Args... args);

		template<class T, typename... Args>
		void registerSimple(const std::string& vKey);
		
	private:
		void __register(const std::string& vKey, Any vConstructor);
		
		template<class T, typename... Args>
		T* __resolve(const std::string& vKey, Args... args);

		std::unordered_map<std::string, Any> m_CreateMap;
	};

//***********************************************************
//FUNCTION::
	template<class T, typename... Args>
	std::shared_ptr<T> CIoc::createProduct(const std::string& vKey, Args... args)
	{
		T* pProduct = __resolve<T>(vKey, args...);

		return std::shared_ptr<T>(pProduct);
	}

//***********************************************************
//FUNCTION::
	template<class T, typename... Args>
	void CIoc::registerSimple(const std::string& vKey)
	{
		std::function<T* (Args...)> CreatorFunc = [](Args... args) {return new T(args...); };
		__register(vKey, CreatorFunc);
	}

//***********************************************************
//FUNCTION::
	void CIoc::__register(const std::string& vKey, Any vConstructor)
	{
		if (m_CreateMap.find(vKey) != m_CreateMap.end())
			throw std::invalid_argument("This product key has already exist.");

		//通过Any擦除了不同类型的构造器
		m_CreateMap.emplace(vKey, vConstructor);
	}

//***********************************************************
//FUNCTION::
	template<class T, typename... Args>
	T* CIoc::__resolve(const std::string& vKey, Args... args)
	{
		if (m_CreateMap.find(vKey) == m_CreateMap.end())
			return nullptr;

		Any Resolver = m_CreateMap[vKey];
		std::function<T* (Args...)> CreatorFunc = Resolver.AnyCast<std::function<T* (Args...)>>();

		return CreatorFunc(args...);
	}

//***********************************************************
//FUNCTION::
	template<class T, typename Depend, typename... Args>
	void CoCo::CIoc::registerType(const std::string& vKey)
	{
		std::function<T* (Args...)> CreatorFunc = [](Args... args){
			return new T(new Depend(args...));
		};//通过闭包擦除了参数类型

		__register(vKey, CreatorFunc);
	}
}