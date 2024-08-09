#include "Scene.h"
#include <algorithm>

using namespace lsmf;


Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

std::unique_ptr<GameObject> Scene::Remove(GameObject* object)
{
	auto it = std::ranges::find_if(m_objects, [object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
	if (it != m_objects.end())
	{
		std::unique_ptr<GameObject> removedObject = std::move(*it);
		m_objects.erase(it);
		return removedObject;
	}
	return nullptr; 
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

GameObject* Scene::Get(const std::string& name) const
{
	auto it = std::ranges::find_if(m_objects, [name](const std::unique_ptr<GameObject>& obj) { return obj->GetName() == name; });
	if (it != m_objects.end())
	{
		return it->get();
	}
	return nullptr;
}

void lsmf::Scene::FixedUpdate(double deltaTime)
{
	if (!m_Active)
		return;
	for (auto& object : m_objects)
	{
		object->FixedUpdate(deltaTime);
	}
}

void Scene::Update(double deltaTime)
{
	if (!m_Active)
		return;
	// remove objects marked for deletion
	std::erase_if(m_objects,
		[](const std::unique_ptr<GameObject>& object)
		{
			return object->IsMarkedForDestruction();
		});

	for (auto& object : m_objects)
	{
		object->Update(deltaTime);
	}
}

void Scene::Render() const
{
	if (!m_Active)
		return;
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

