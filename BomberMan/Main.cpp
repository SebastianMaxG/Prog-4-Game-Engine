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
#include "GlobalSignals.h"
#include "OrbitComponent.h"
#include "SoundSystem.h"


lsmf::Minigin* g_engine;

void load()
{
	int windowX{}, windowY{};
	SDL_GetWindowSize(g_engine->GetSDLWindow(), &windowX, &windowY);
	[[maybe_unused]] auto& scene = lsmf::SceneManager::GetInstance().CreateScene("Demo");

	lsmf::Transform transform{};
	auto font = lsmf::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto go = std::make_unique<lsmf::GameObject>();
	go->AddComponent(std::move(std::make_unique<lsmf::SpriteComponent>(go.get(), "background.tga")));
	scene.Add(std::move(go));

	go = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({ 216, 180, -1 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::SpriteComponent>(go.get(), "logo.tga")));
	scene.Add(std::move(go));

	go = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({ 80, 20, 0 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::TextComponent>(go.get(), "Programming 4 Assignment", font)));
	scene.Add(std::move(go));

	go = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({ 10, 60, 0 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::FPSComponent>(go.get(), font)));
	scene.Add(std::move(go));


	go = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({windowX/2.f, windowY/2.f, 0 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::SpriteComponent>(go.get(), "logo.tga")));
	go->AddComponent(std::move(std::make_unique<lsmf::OrbitComponent>(go.get(),1,100,0)));

	auto go1 = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({0, 10, 0 });
	go1->GetTransform()->SetPosition(transform.GetPosition());
	go1->AddComponent(std::move(std::make_unique<lsmf::OrbitComponent>(go1.get(),1,100,0)));
	go1->AddComponent(std::move(std::make_unique<lsmf::SpriteComponent>(go1.get(), "logo.tga")));
	go1->SetParent(go.get(), false);

	
	scene.Add(std::move(go));
	scene.Add(std::move(go1));

	//go = std::make_shared<lsmf::GameObject>();
	//go->SetTexture("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//auto to = std::make_shared<lsmf::TextObject>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//scene.Add(to);

	//auto fps = std::make_shared<lsmf::TextObject>("* fps", font, SDL_Color(255,255,0,255));
	//fps->SetPosition(80, 20);
	//scene.Add(fps);
}

int main(int, char*[]) {
	g_engine = new lsmf::Minigin("../Data/");
	g_engine->Run(load);
	lsmf::sound::PlaySoundSignal.End();
	lsmf::globalSignals::OnEnemyDeath.End();

	delete g_engine;
    return 0;
}