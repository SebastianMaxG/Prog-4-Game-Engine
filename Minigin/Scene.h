#pragma once
#include "GameObject.h"

namespace lsmf
{

	class Scene final
	{
	public:
		std::string GetName() const	{ return m_name;	}
		void SetActive(bool active) { m_Active = active; }
		void MarkForDestruction()
		{
			m_MarkedForDestuction = true;
			RemoveAll();
		}
		bool IsMarkedForDestruction() const { return m_MarkedForDestuction; }

		void Add(std::unique_ptr<GameObject> object);
		std::unique_ptr<GameObject> Remove(GameObject* object);
		void RemoveAll() const;
		//get game object by name
		 GameObject* Get(const std::string& name) const;

		void FixedUpdate(double deltaTime) const;
		void Update(double deltaTime);
		void Render() const;

		Scene(std::string name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 

		bool m_Active{true};
		bool m_MarkedForDestuction{false};
		std::string m_name;
		std::vector <std::unique_ptr<GameObject>> m_objects{};
	};

}
