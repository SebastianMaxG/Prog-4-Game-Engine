#pragma once
#include <string>
#include <memory>
#include <functional>
#include <SDL.h>

namespace lsmf
{
	class GameObject;
	class TransformComponent;

	class BaseComponent
	{
	public:
		BaseComponent(GameObject* m_GameObjectPtr);
		virtual ~BaseComponent() = default;

		void DoUpdate(double deltaTime);
		void DoFixedUpdate(double deltaTime);
		void DoRender() const;
	protected:
		virtual void Update(double deltaTime);
		virtual void FixedUpdate(double deltaTime);
		virtual void Render() const;
	public:
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;


		void Start();
		void Stop();
		//virtual void Destroy() = 0;
		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }
		GameObject* GetGameObject() { return m_GameObjectPtr; };
		const GameObject* GetGameObject() const { return m_GameObjectPtr; }; 

	//	void SetTransformComponent(std::weak_ptr<lsmf::TransformComponent> transformComponent)
	//	{
	//		m_TransformComponent.swap(transformComponent);
	//	};
	//protected:

	private:
		void Initialize();
		GameObject* m_GameObjectPtr;

		bool m_IsInitialized;
		bool m_IsActive;

		std::string m_Name;

	};
}