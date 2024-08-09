#include "SceneManager.h"
#include "Scene.h" // Include Scene.h

void lsmf::SceneManager::Update(double deltaTime)
{
	RemoveDeletedScenes();
	for (auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void lsmf::SceneManager::FixedUpdate(double deltaTime)
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate(deltaTime);
	}
}

void lsmf::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void lsmf::SceneManager::RemoveDeletedScenes()
{
	for (auto scene : m_ScenesToBeDeleted)
	{
		auto it = std::ranges::find_if(m_scenes, [scene](const std::unique_ptr<Scene>& scenePtr) {return scenePtr.get() == scene; });
		if (it != m_scenes.end())
		{
			m_scenes.erase(it);
		}
	}
	m_ScenesToBeDeleted.clear();
}

lsmf::Scene* lsmf::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	Scene* scenePtr = scene.get();
	m_scenes.push_back(std::move(scene));
	return scenePtr;
}

void lsmf::SceneManager::DeleteScene(const std::string& name)
{
	auto it = std::ranges::find_if(m_scenes, [&name](const std::unique_ptr<Scene>& scene) {return scene->GetName() == name; });
	if (it != m_scenes.end())
	{
		m_ScenesToBeDeleted.push_back(it->get());
	}
}

lsmf::Scene* lsmf::SceneManager::GetScene(const std::string& name)
{
	auto it = std::ranges::find_if(m_scenes, [&name](const std::unique_ptr<Scene>& scene) {return scene->GetName() == name; });
	if (it != m_scenes.end())
	{
		return it->get();
	}
	return nullptr;
}
