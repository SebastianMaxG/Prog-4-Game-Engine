#pragma once
#include <string>
#include <memory>
#include <functional>
#include <SDL.h>
//todo: Send Signal
namespace dae
{
	class GameObject;
	class TransformComponent;

	class BaseComponent
	{
	public:
		BaseComponent(GameObject* m_GameObjectPtr);
		virtual ~BaseComponent() = default;

		//make it so that the inherited update function has to call this update function
		
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
		BaseComponent& operator=(BaseComponent&& other);


		void Start();
		void Stop();
		//virtual void Destroy() = 0;
		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }
		const GameObject* GetGameObject() { return m_GameObjectPtr; };

	//	void SetTransformComponent(std::weak_ptr<dae::TransformComponent> transformComponent)
	//	{
	//		m_TransformComponent.swap(transformComponent);
	//	};
	//protected:
	//	std::weak_ptr<TransformComponent> m_TransformComponent;

	private:
		virtual void Initialize();
		bool m_IsInitialized;
		bool m_IsActive; //todo: make this a state / only update when active

		const GameObject* m_GameObjectPtr;
		std::string m_Name;

	};
}
//public:
//	BaseComponent() = default;
//	virtual ~BaseComponent();
//	template<typename T>
//	bool ReceiveSignal(std::string signal, T &data = 0);
//
//protected:
//	//make void* function pointer
//	//std::map<std::string, void*> m_Signals{};
//	std::map<std::string, std::function<void(void*)>> m_Signals{};
//	// todo messages and references to Transform component etc