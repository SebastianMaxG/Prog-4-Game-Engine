#include "SceneManager.h"
#include "Scene.h" // Include Scene.h

void lsmf::SceneManager::Update(double deltaTime)
{
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

lsmf::Scene* lsmf::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	Scene* scenePtr = scene.get();
	m_scenes.push_back(std::move(scene));
	return scenePtr;
}