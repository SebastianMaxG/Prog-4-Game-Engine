#include "Scene.h"
#include <algorithm>

using namespace lsmf;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* object)
{
	auto it = std::ranges::find_if(m_objects, [object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
	if (it != m_objects.end())
	{
		m_objects.erase(it);
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void lsmf::Scene::FixedUpdate(double deltaTime)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(deltaTime);
	}
}

void Scene::Update(double deltaTime)
{
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
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

