#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"
#include "FPSComponent.h"

dae::Minigin* g_engine;

void load()
{
	int windowX{}, windowY{};
	SDL_GetWindowSize(g_engine->GetSDLWindow(), &windowX, &windowY);
	[[maybe_unused]] auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	dae::Transform transform{};
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto go = std::make_shared<dae::GameObject>();
	//go->AddComponent(std::make_shared<dae::TransformComponent>(go.get()));
	go->AddComponent(std::make_shared<dae::SpriteComponent>(go.get(), "background.tga"));
	scene.Add(go);
	go = std::make_shared<dae::GameObject>();
	transform.SetPosition({ 216, 180, 0 });
	go->AddComponent(std::make_shared<dae::SpriteComponent>(go.get(), "logo.tga", transform));
	scene.Add(go);
	transform.SetPosition({ 80, 20, 0 });
	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_shared<dae::TextComponent>(go.get(), "Programming 4 Assignment", font, transform));
	scene.Add(go);
	go = std::make_shared<dae::GameObject>();
	transform.SetPosition({ 10, 60, 0 });
	go->AddComponent(std::make_shared<dae::FPSComponent>(go.get(), font, transform));
	scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->SetTexture("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//scene.Add(to);

	//auto fps = std::make_shared<dae::TextObject>("* fps", font, SDL_Color(255,255,0,255));
	//fps->SetPosition(80, 20);
	//scene.Add(fps);
}

int main(int, char*[]) {
	g_engine = new dae::Minigin("../Data/");
	g_engine->Run(load);
	delete g_engine;
    return 0;
}