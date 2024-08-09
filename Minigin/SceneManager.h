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

		void Update(double deltaTime);
		void FixedUpdate(double deltaTime);
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes;
	};
}
