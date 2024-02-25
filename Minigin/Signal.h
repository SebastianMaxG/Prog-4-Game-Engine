#pragma once
#include <functional>
#include <set>
#include <vector>


template<typename... Args>
class Signal
{
	typedef std::function<void(Args...)> functionType;
	public:
		Signal() = default;
		~Signal();

		void AddListener(functionType listener) { m_ListenerFunctions.insert(listener); }
		void RemoveListener(functionType listener) { m_ListenerFunctions.erase(listener); }
		void Emit(Args... args);
	private:
		std::set<functionType> m_ListenerFunctions{};
};

template<typename ...Args>
inline Signal<Args...>::~Signal()
{
	m_ListenerFunctions.clear();
}

template<typename ...Args>
inline void Signal<Args...>::Emit(Args ...args)
{
	for (auto& listenerFunction : m_ListenerFunctions)
	{
		listenerFunction(args...);
	}
}