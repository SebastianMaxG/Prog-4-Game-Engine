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
#include "Player.h"
#include "SoundSystem.h"
#include "Tile.h"
#include "TileGrid.h"


lsmf::Minigin* g_engine;
const std::string DATAPATH{ "../Data/" };

void load()
{
	int windowX{}, windowY{};
	SDL_GetWindowSize(g_engine->GetSDLWindow(), &windowX, &windowY);
	[[maybe_unused]] auto& scene = lsmf::SceneManager::GetInstance().CreateScene("Demo");

	lsmf::Transform transform{};
	auto font = lsmf::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	/*auto go = std::make_unique<lsmf::GameObject>();
	go->AddComponent(std::move(std::make_unique<lsmf::SpriteComponent>(go.get(), "background.tga")));
	scene.Add(std::move(go));

	go = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({ 216, 180, 1 });
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
	go1->AddComponent(std::move(std::make_unique<lsmf::SpriteComponent>(go1.get(), "PlayerSprite.png")));
	go1->SetParent(go.get(), false);

	
	scene.Add(std::move(go));
	scene.Add(std::move(go1));*/

	auto go = std::make_unique<lsmf::GameObject>();
	auto grid = std::make_unique<lsmf::TileGrid>(go.get());
	grid->LoadFromFile(DATAPATH+"Level1.txt", scene);

	go->AddComponent(std::move(grid));
	scene.Add(std::move(go));


	 go = std::make_unique<lsmf::GameObject>();
	transform.SetPosition({ 16, 16, 2 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::Player>(go.get())));
	scene.Add(std::move(go));

	//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ 0, 0, 1 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->AddComponent(std::move(std::make_unique<lsmf::Tile>(go.get(), lsmf::Tile::TileType::Wall)));
	//scene.Add(std::move(go));
	//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ 0, 16, 1 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->AddComponent(std::move(std::make_unique<lsmf::Tile>(go.get(), lsmf::Tile::TileType::Wall)));
	//scene.Add(std::move(go));
	//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ 0, 32, 1 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->AddComponent(std::move(std::make_unique<lsmf::Tile>(go.get(), lsmf::Tile::TileType::Wall)));
	//scene.Add(std::move(go));
	//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ 16, 0, 1 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->AddComponent(std::move(std::make_unique<lsmf::Tile>(go.get(), lsmf::Tile::TileType::Wall)));
	//scene.Add(std::move(go));
	//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ 32, 0, 1 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->AddComponent(std::move(std::make_unique<lsmf::Tile>(go.get(), lsmf::Tile::TileType::Wall)));
	//scene.Add(std::move(go));
	//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ 32, 32, 1 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->AddComponent(std::move(std::make_unique<lsmf::Tile>(go.get(), lsmf::Tile::TileType::Wall)));
	//scene.Add(std::move(go));
}

int main(int, char*[]) {
	g_engine = new lsmf::Minigin(DATAPATH);
	g_engine->Run(load);
	lsmf::sound::PlaySoundSignal.End();
	lsmf::globalSignals::OnEnemyDeath.End();

	delete g_engine;
    return 0;
}