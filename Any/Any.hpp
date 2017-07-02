#pragma once
#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>

namespace CoCo
{
	class CAny
	{
	public:
		CAny(void) : m_TypeIndex(std::type_index(typeid(void))) {}
		CAny(CAny& vRhs) : m_Ptr(vRhs.__clone()), m_TypeIndex(vRhs.m_TypeIndex) {}
		CAny(CAny&& vRhs) : m_Ptr(std::move(vRhs.m_Ptr)), m_TypeIndex(vRhs.m_TypeIndex) {}

		//创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
		template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, CAny>::value, U>::type> CAny(U && value)
			: m_Ptr(new CDerived < typename std::decay<U>::type>(std::forward<U>(value))),
			m_TypeIndex(std::type_index(typeid(typename std::decay<U>::type))) {}


		CAny& operator=(const CAny& vRhs)
		{
			if (m_Ptr == vRhs.m_Ptr)
				return *this;
			m_Ptr = vRhs.__clone();
			m_TypeIndex = vRhs.m_TypeIndex;
			return *this;
		}
		~CAny(){}



		bool isNull() const { return !bool(m_Ptr); }

	private:
		class CBase;
		typedef std::unique_ptr<CBase> BasePtr;

		class CBase
		{
		public:
			virtual ~CBase() {}
			virtual BasePtr clone() const = 0;
		};

		template<typename T>
		class CDerived : public CBase
		{
		public:
			template<typename U>
			CDerived(U&& vValue) : m_Value(std::forward<U>(vValue)) 
			{

			}

			virtual BasePtr clone() const override
			{
				return BasePtr(new CDerived<T>(m_Value));
			}

			T m_Value;
		};

		BasePtr __clone() const
		{
			if (m_Ptr != nullptr)
				return m_Ptr->clone();

			return nullptr;
		}

		BasePtr m_Ptr;
		std::type_index m_TypeIndex;
	};
}