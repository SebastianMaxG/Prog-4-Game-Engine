#include "SceneManager.h"
#include "Scene.h" // Include Scene.h

void lsmf::SceneManager::Update(double deltaTime)
{
	for (const auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
	RemoveDeletedScenes();
}

void lsmf::SceneManager::FixedUpdate(double deltaTime) const
{
	for (const auto& scene : m_scenes)
	{
		scene->FixedUpdate(deltaTime);
	}
}

void lsmf::SceneManager::Render() const
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void lsmf::SceneManager::RemoveDeletedScenes()
{
	std::erase_if(m_scenes, [](const std::unique_ptr<Scene>& scene) {return scene->IsMarkedForDestruction(); });

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
		it->get()->MarkForDestruction();
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

void lsmf::SceneManager::ClearScenes()
{
	m_scenes.clear();
}
