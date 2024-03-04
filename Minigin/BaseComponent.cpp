#include "BaseComponent.h"
#include <algorithm>
#include <iostream>
#include "GameObject.h"



dae::BaseComponent::BaseComponent(GameObject* gameObjectPtr)
	:m_GameObjectPtr(gameObjectPtr)
	, m_IsActive(false)
	, m_IsInitialized(false)
{
	Initialize();
}

void dae::BaseComponent::DoUpdate(double deltaTime)
{
	if (m_IsActive)
	{
		Update(deltaTime);
	}
}

void dae::BaseComponent::DoFixedUpdate(double deltaTime)
{
	if (m_IsActive)
	{
		FixedUpdate(deltaTime);
	}
}

void dae::BaseComponent::DoRender() const
{
	if (m_IsActive)
	{
		Render();
	}
}

void dae::BaseComponent::Update([[maybe_unused]] double deltaTime)
{

}

void dae::BaseComponent::FixedUpdate([[maybe_unused]] double deltaTime)
{
}

void dae::BaseComponent::Render() const
{
}

dae::BaseComponent& dae::BaseComponent::operator=(BaseComponent&& other)
{
	if (this != &other)
	{
		m_GameObjectPtr = other.m_GameObjectPtr;
		m_IsActive = other.m_IsActive;
		m_IsInitialized = other.m_IsInitialized;
		other.m_IsActive = false;
		other.m_IsInitialized = false;
		other.m_GameObjectPtr = nullptr;
	}
	return *this;
}

void dae::BaseComponent::Start()
{
	m_IsActive = true;
}

void dae::BaseComponent::Stop()
{
	m_IsActive = false;
}

void dae::BaseComponent::Initialize()
{
	m_IsInitialized = true;
	m_IsActive = true;
}


//template<typename T>
//bool BaseComponent::ReceiveSignal(std::string signal, T& data)
//{
//	if (m_Signals.empty())
//	{
//		return false;
//	}
//	if (m_Signals.contains(signal))
//	{
//		//call function in map with data
//		auto it = m_Signals.find(signal);
//		if (it != m_Signals.end())
//		{
//			//call function
//			try
//			{
//				it->second(data);
//			}
//			catch (const std::exception&)
//			{
//				std::cout << "Error in BaseComponent::ReceiveSignal: Could not call function in map" << std::endl;
//			}
//		}
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//}
//
//BaseComponent::~BaseComponent()
//{
//	m_Signals.clear();
//}