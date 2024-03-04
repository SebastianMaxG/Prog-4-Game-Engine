#pragma once
#include <memory>
#include "Transform.h"
#include <set>

namespace dae
{
	class Texture2D;
	class BaseComponent;
	class VisualComponent;
	class PhysicsComponent; 
	class TransformComponent;

	// todo: this should become final.
	class GameObject final
	{
	public:
		 void FixedUpdate(double deltaTime);
		 void Update(double deltaTime);
		 //void LateUpdate(double deltaTime);
		 void Render() const;

		GameObject() = default;
		 ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void AddComponent(std::shared_ptr<BaseComponent> componentPtr);
		void RemoveComponent(std::shared_ptr<BaseComponent> componentPtr);
		const BaseComponent* GetComponent(const std::string& name);
		const BaseComponent* GetComponent(const std::type_info& type);
		const TransformComponent* GetTransform() const { return m_TransformComponent.get(); }

		void SetParent(GameObject* parentPtr, bool keepWorldPosition);
		GameObject* GetParent() const { return m_ParentPtr; };

		void SetDirty();
	private:
		std::set<std::shared_ptr<BaseComponent>> m_Components;
		std::set<std::shared_ptr<VisualComponent>> m_VisualComponents;
		std::set<std::shared_ptr<PhysicsComponent>> m_PhysicsComponents;
		std::shared_ptr<TransformComponent> m_TransformComponent;

		GameObject* m_ParentPtr = nullptr;
		std::set<GameObject*> m_ChildrenPtrs;


		void AddChild(GameObject* childPtr);
		void RemoveChild(GameObject* childPtr);

	};
}
