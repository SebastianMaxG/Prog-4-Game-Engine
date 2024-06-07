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

lsmf::Scene& lsmf::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}