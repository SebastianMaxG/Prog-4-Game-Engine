#pragma once
#include <memory>
#include <set>
#include "Signal.h"
namespace lsmf
{

	class Texture2D;
	class BaseComponent;
	class VisualComponent;
	class PhysicsComponent; 
	class TransformComponent;


	class GameObject final
	{
	public:

		 void FixedUpdate(double deltaTime);
		 void Update(double deltaTime);
		 void LateUpdate(double deltaTime);
		 void Render() const;

		GameObject();
		 ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void AddComponent(std::unique_ptr<BaseComponent> componentPtr);
		void RemoveComponent(std::unique_ptr<BaseComponent> componentPtr);
		BaseComponent* GetComponent(const std::string& name);
		BaseComponent* GetComponent(const std::type_info& type);
		TransformComponent* GetTransform() { return m_TransformComponent.get(); }
		const TransformComponent* GetTransform() const { return m_TransformComponent.get(); }

		void SetParent(GameObject* parentPtr, bool keepWorldPosition);
		GameObject* GetParent() const { return m_ParentPtr; };

		void SetDirty();
		void MarkForDestruction();
		bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }
	private:
		std::set<std::unique_ptr<BaseComponent>> m_Components;
		std::unique_ptr<TransformComponent> m_TransformComponent;

		GameObject* m_ParentPtr = nullptr;
		std::set<GameObject*> m_ChildrenPtrs;


		void AddChild(GameObject* childPtr);
		void RemoveChild(GameObject* childPtr);

		bool m_MarkedForDestruction = false;


	};
}
