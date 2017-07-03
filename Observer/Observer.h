#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <map>

namespace CoCo
{
	template<typename Func>
	class CObserver
	{
	public:
		CObserver(){}
		~CObserver(){}

		int attach(const Func& vFunc); //ע��۲���
		int attach(Func&& vFunc);

		void detach(int vObserverId); //�Ƴ��۲���

		template<typename... Args>
		void notify(Args... args)
		{
			for (auto& observer : m_Connections)
			{
				observer.second(std::forward<Args>(args)...);
			}
		}

	private:
		template<typename F>
		int __assign(F&& vF)
		{
			int ObserverId = m_ObserverId++;
			m_Connections.emplace(ObserverId, std::forward<F>(vF));
			return ObserverId;
		}

		int m_ObserverId;
		std::map<int, Func> m_Connections;
	};

	template<typename Func>
	void CoCo::CObserver<Func>::detach(int vObserverId)
	{
		m_Connections.erase(vObserverId);
	}

	template<typename Func>
	int CoCo::CObserver<Func>::attach(Func&& vFunc)
	{
		return __assign(vFunc);
	}

	template<typename Func>
	int CoCo::CObserver<Func>::attach(const Func& vFunc)
	{
		return __assign(vFunc);
	}

}
