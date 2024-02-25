#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "VisualComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

namespace dae
{
	GameObject::~GameObject() = default;

	void GameObject::FixedUpdate(double deltaTime)
	{
		for (const auto& component : m_PhysicsComponents)
		{
			component->DoFixedUpdate(deltaTime);
		}
	}

	void GameObject::Update(double deltaTime) 
	{
		for (const auto& component : m_Components)
		{
			component->DoUpdate(deltaTime);
		}
		for (const auto& component : m_PhysicsComponents)
		{
			component->DoUpdate(deltaTime);
		}
		for (const auto& component : m_VisualComponents)
		{
			component->DoUpdate(deltaTime);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_VisualComponents)
		{
			component->DoRender();
		}
	}

	void GameObject::AddComponent(std::shared_ptr<BaseComponent> componentPtr)
	{
		// check if the component is a visual component
		if (auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(componentPtr))
		{
			m_VisualComponents.insert(visualComponent);
		}
		else if (auto physicsComponent = std::dynamic_pointer_cast<PhysicsComponent>(componentPtr))
		{
			m_PhysicsComponents.insert(physicsComponent);
		}
		else if (auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(componentPtr))
		{
			m_TransformComponent.swap(transformComponent);
		}
		else
		{
			m_Components.insert(componentPtr);
		}
		componentPtr->SetGameObject(this);
		if (m_TransformComponent)
			componentPtr->SetTransformComponent(std::weak_ptr(m_TransformComponent));
	}

	void GameObject::RemoveComponent(std::shared_ptr<BaseComponent> componentPtr)
	{
		m_Components.erase(componentPtr);
	}

	const BaseComponent* GameObject::GetComponent(const std::string& name)
	{
		for (const auto& component : m_Components)
		{
			if (component->GetName() == name)
			{
				return component.get();
			}
		}
		for (const auto& component : m_PhysicsComponents)
		{
			if (component->GetName() == name)
			{
				return component.get();
			}
		}
		for (const auto& component : m_VisualComponents)
		{
			if (component->GetName() == name)
			{
				return component.get();
			}
		}
		return nullptr;
	}

	// returns the first component of the given type
	const BaseComponent* GameObject::GetComponent(const std::type_info& type)
	{
		for (const auto& component : m_Components)
		{
			if (typeid(*component) == type)
			{
				return component.get();
			}
		}
		for (const auto& component : m_PhysicsComponents)
		{
			if (typeid(*component) == type)
			{
				return component.get();
			}
		}
		for (const auto& component : m_VisualComponents)
		{
			if (typeid(*component) == type)
			{
				return component.get();
			}
		}
		return nullptr;
	}
}