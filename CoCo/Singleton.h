#pragma once

namespace CoCo
{
	template<typename T>
	class CSingleton
	{
	public:
		template<typename... Args>
		static T* Instance(Args&&... args)
		{
			if (nullptr == m_pInstance)
				m_pInstance = new T(std::forward<Args>(args)...); //完美转发

			return m_pInstance;
		}

		static T* getInstance() const;
		static void destroyInstance();

	private:
		CSingleton();
		virtual ~CSingleton();

		static T* m_pInstance;
	};

	template<typename T>
	void CoCo::CSingleton<T>::destroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	template<typename T>
	T* CoCo::CSingleton<T>::getInstance() const
	{
		if (m_pInstance == nullptr)
			throw std::logic_error("The instance is not init, please initialize the instance first");

		return m_pInstance;
	}

	template<typename T>
	T* CoCo::CSingleton<T>::m_pInstance = nullptr;
}

