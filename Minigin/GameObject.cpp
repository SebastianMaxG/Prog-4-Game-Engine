#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include <iostream>

namespace lsmf
{
	GameObject::GameObject()
	{
		m_TransformComponent = std::make_unique<TransformComponent>(this);
	}
	GameObject::~GameObject()
	{
		m_Components.clear();
		m_ChildrenPtrs.clear();
		m_ParentPtr = nullptr;
	}

	void GameObject::FixedUpdate(double deltaTime)
	{
		if (m_MarkedForDestruction)
		{
			return;
		}
		for (const auto& component : m_Components)
		{
			component->DoFixedUpdate(deltaTime);
		}
	}

	void GameObject::Update(double deltaTime) 
	{

		if (m_MarkedForDestruction)
		{
			return;
		}
		for (const auto& component : m_Components)
		{
			component->DoUpdate(deltaTime);
		}
	}

	void GameObject::Render() const
	{

		if (m_MarkedForDestruction)
		{
			return;
		}
		for (const auto& component : m_Components)
		{
			component->DoRender();
		}
	}

	void GameObject::AddComponent(std::unique_ptr<BaseComponent> componentPtr)
	{
		if (componentPtr->GetGameObject() != this)
		{
			return;
		}
		if (const TransformComponent* transform = dynamic_cast<TransformComponent*>(componentPtr.get()))
		{
			m_TransformComponent->SetLocalTransform(transform->GetLocalTransform());
		}
		else
		{
			m_Components.insert(std::move(componentPtr));
		}
	}

	void GameObject::RemoveComponent(std::unique_ptr<BaseComponent> componentPtr)
	{
		m_Components.erase(componentPtr);
	}

	BaseComponent* GameObject::GetComponent(const std::string& name)
	{
		for (const auto& component : m_Components)
		{
			if (component->GetName() == name)
			{
				return component.get();
			}
		}
		for (const auto& component : m_Components)
		{
			if (component->GetName() == name)
			{
				return component.get();
			}
		}
		for (const auto& component : m_Components)
		{
			if (component->GetName() == name)
			{
				return component.get();
			}
		}
		return nullptr;
	}

	// returns the first component of the given type
	BaseComponent* GameObject::GetComponent(const std::type_info& type) const
	{
		for (const auto& component : m_Components)
		{
			if (typeid(*component.get()) == type)
			{
				return component.get();
			}
		}
		return nullptr;
	}
	void GameObject::SetParent(GameObject* parentPtr, bool keepWorldPosition)
	{
		if (parentPtr == this or m_ChildrenPtrs.contains(parentPtr) or m_ParentPtr == parentPtr)
		{
			return;
		}
		if (m_TransformComponent)
		{
			if (parentPtr == nullptr)
			{
				m_TransformComponent->SetLocalTransform(m_TransformComponent->GetWorldTransform());
				m_TransformComponent->SetIsRoot(true);
			}
			else
			{
				if (keepWorldPosition)
				{
					m_TransformComponent->SetLocalTransform
					(m_TransformComponent->GetWorldTransform() 
						- m_ParentPtr->GetTransform()->GetWorldTransform());
				}
				SetDirty();
				m_TransformComponent->SetIsRoot(false);
			}
		}
		if (m_ParentPtr)
		{
			m_ParentPtr->RemoveChild(this);
		}
		m_ParentPtr = parentPtr;
		if (m_ParentPtr)
		{
			m_ParentPtr->AddChild(this);
		}
	}
	void GameObject::SetDirty()
	{
		for (GameObject* child : m_ChildrenPtrs)
		{
			child->SetDirty();
		}
		if (m_TransformComponent)
		{
			m_TransformComponent->SetDirty();
		}
	}
	void GameObject::MarkForDestruction()
	{
		m_MarkedForDestruction = true;
		for (GameObject* child : m_ChildrenPtrs)
		{
			child->MarkForDestruction();
		}
		if (m_ParentPtr)
		{
			m_ParentPtr->RemoveChild(this);
		}
		m_ParentPtr = nullptr;
	}
	void GameObject::AddChild(GameObject* childPtr)
	{
		m_ChildrenPtrs.emplace(childPtr);
	}
	void GameObject::RemoveChild(GameObject* childPtr)
	{
		m_ChildrenPtrs.erase(childPtr);
	}
}