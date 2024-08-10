#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"

#include "Singleton.h"

namespace lsmf
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);
		void DeleteScene (const std::string& name);
		Scene* GetScene(const std::string& name);
		void ClearScenes();

		void Update(double deltaTime);
		void FixedUpdate(double deltaTime) const;
		void Render() const;
	private:
		void RemoveDeletedScenes();
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes;

		std::vector<Scene*> m_ScenesToBeDeleted;
	};
}
