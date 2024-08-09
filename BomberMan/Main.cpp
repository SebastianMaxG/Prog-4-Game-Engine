#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>

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
#include "GlobalEngineSignals.h"
#include "GlobalSignals.h"
#include "MainMenuComponent.h"
#include "OrbitComponent.h"
#include "Player.h"
#include "ScoreSystem.h"
#include "SoundSystem.h"
#include "Tile.h"
#include "TileGrid.h"


lsmf::Minigin* g_engine;
lsmf::SoundSystem* g_sound_system;
const std::string DATAPATH{ "../Data/" };

namespace level
{
	int g_CurrentLevel{ 1 };

	void NextLevel()
	{

		//get the player(s) and the score system and create the next level
		auto level = lsmf::SceneManager::GetInstance().GetScene("Level" + std::to_string(g_CurrentLevel));
		auto player1 = level->Get("Player1");
		auto player2 = level->Get("Player2");
		auto scoreSystem = level->Get("ScoreSystem");

		std::unique_ptr<lsmf::GameObject> Player1;
		if (player1)
		{
			Player1 = level->Remove(player1);
		}

		std::unique_ptr<lsmf::GameObject> Player2;
		if (player2)
		{
			Player2 = level->Remove(player2);
		}

		std::unique_ptr<lsmf::GameObject> ScoreSystem;
		if (scoreSystem)
		{
			ScoreSystem = level->Remove(scoreSystem);
		}

		lsmf::SceneManager::GetInstance().DeleteScene("Level" + std::to_string(g_CurrentLevel));
		++g_CurrentLevel;

		//Check if the next level exists
		if (g_CurrentLevel > 5)
		{
			g_CurrentLevel = 1; //reset to level 1
			lsmf ::SceneManager::GetInstance().GetScene("Menu")->SetActive(true);
			return;
		}

		level = lsmf::SceneManager::GetInstance().CreateScene("Level" + std::to_string(g_CurrentLevel));

		auto go = std::make_unique<lsmf::GameObject>();
		auto grid = std::make_unique<lsmf::TileGrid>(go.get(), glm::vec2{ -8,40 });
		lsmf::TileGrid* tileGrid = grid.get();
		//grid->LoadFromFile(DATAPATH+"Level1.txt", scene);
		grid->GenerateRandom(g_CurrentLevel, level);

		go->AddComponent(std::move(grid));
		level->Add(std::move(go));

		if (Player1)
		{
			auto playerStart = tileGrid->GetTile(1, 1)->GetGameObject()->GetTransform()->GetPosition();

			Player1->GetTransform()->SetPosition({ playerStart.x, playerStart.y, 2 });
			lsmf::Player* player = dynamic_cast<lsmf::Player*>(Player1->GetComponent(typeid(lsmf::Player)));
			player->Reset(tileGrid);
			level->Add(std::move(Player1));
		}
		if (Player2)
		{
			auto playerStart = tileGrid->GetTile(1, 2)->GetGameObject()->GetTransform()->GetPosition();

			Player2->GetTransform()->SetPosition({ playerStart.x, playerStart.y, 2 });
			lsmf::Player* player = dynamic_cast<lsmf::Player*>(Player2->GetComponent(typeid(lsmf::Player)));
			player->Reset(tileGrid);
			level->Add(std::move(Player2));
		}
		if (ScoreSystem)
		{
			ScoreSystem->GetTransform()->SetPosition({ 80, 20, 10 });
			level->Add(std::move(ScoreSystem));
		}
	}
}

void LoadSingleplayer()
{
	lsmf::SceneManager::GetInstance().GetScene("Menu")->SetActive(false);

	//todo: load singleplayer (one player)
	std::cout << "Singleplayer" << std::endl;

	const auto level = lsmf::SceneManager::GetInstance().CreateScene("Level" + std::to_string(level::g_CurrentLevel));

	lsmf::Transform transform{};
	transform.SetScale({ 1,1,1 });

	auto go = std::make_unique<lsmf::GameObject>();
	auto grid = std::make_unique<lsmf::TileGrid>(go.get(), glm::vec2{ -8,40 });
	lsmf::TileGrid* tileGrid = grid.get();
	//grid->LoadFromFile(DATAPATH+"Level1.txt", scene);
	grid->GenerateRandom(1, level);

	go->AddComponent(std::move(grid));
	level->Add(std::move(go));

	auto playerStart = tileGrid->GetTile(1, 1)->GetGameObject()->GetTransform()->GetPosition();

	go = std::make_unique<lsmf::GameObject>();
	go->SetName("Player1");
	transform.SetPosition({ playerStart.x, playerStart.y, 2 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::Player>(go.get(), tileGrid, 0,true)));
	level->Add(std::move(go));


	auto go2 = std::make_unique<lsmf::GameObject>();
	go2->SetName("ScoreSystem");
	transform.SetPosition({ 80, 20, 10 });
	go2->GetTransform()->SetPosition(transform.GetPosition());
	go2->AddComponent(std::move(std::make_unique<lsmf::ScoreSystem>(go2.get())));
	level->Add(std::move(go2));
}
void LoadCoop()
{
	lsmf::SceneManager::GetInstance().GetScene("Menu")->SetActive(false);

	//todo: load coop (two players)
	std::cout << "Coop" << std::endl;
	const auto level = lsmf::SceneManager::GetInstance().CreateScene("Level" + std::to_string(level::g_CurrentLevel));

	lsmf::Transform transform{};
	transform.SetScale({ 1,1,1 });

	auto go = std::make_unique<lsmf::GameObject>();
	auto grid = std::make_unique<lsmf::TileGrid>(go.get(), glm::vec2{ -8,40 });
	lsmf::TileGrid* tileGrid = grid.get();
	//grid->LoadFromFile(DATAPATH+"Level1.txt", scene);
	grid->GenerateRandom(1, level);

	go->AddComponent(std::move(grid));
	level->Add(std::move(go));

	auto playerStart = tileGrid->GetTile(1, 1)->GetGameObject()->GetTransform()->GetPosition();

	go = std::make_unique<lsmf::GameObject>();
	go->SetName("Player1");
	transform.SetPosition({ playerStart.x, playerStart.y, 2 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::Player>(go.get(), tileGrid, 0)));
	level->Add(std::move(go));

	playerStart = tileGrid->GetTile(1, 2)->GetGameObject()->GetTransform()->GetPosition();

	go = std::make_unique<lsmf::GameObject>();
	go->SetName("Player2");
	transform.SetPosition({ playerStart.x, playerStart.y, 2 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::Player>(go.get(), tileGrid, -1)));
	level->Add(std::move(go));


	auto go2 = std::make_unique<lsmf::GameObject>();
	go2->SetName("ScoreSystem");
	transform.SetPosition({ 80, 20, 10 });
	go2->GetTransform()->SetPosition(transform.GetPosition());
	go2->AddComponent(std::move(std::make_unique<lsmf::ScoreSystem>(go2.get())));
	level->Add(std::move(go2));
}
void LoadVersus()
{
	lsmf::SceneManager::GetInstance().GetScene("Menu")->SetActive(false);

	//todo: load versus (one players, one enemy with player controller)
	std::cout << "Versus" << std::endl;

	const auto level = lsmf::SceneManager::GetInstance().CreateScene("Level" + std::to_string(level::g_CurrentLevel));
	lsmf::Transform transform{};


	auto go2 = std::make_unique<lsmf::GameObject>();
	go2->SetName("ScoreSystem");
	transform.SetPosition({ 80, 20, 10 });
	go2->GetTransform()->SetPosition(transform.GetPosition());
	go2->AddComponent(std::move(std::make_unique<lsmf::ScoreSystem>(go2.get())));
	level->Add(std::move(go2));
}



void load()
{
	int windowX{}, windowY{};
	SDL_GetWindowSize(g_engine->GetSDLWindow(), &windowX, &windowY);
	const auto sceneMenu = lsmf::SceneManager::GetInstance().CreateScene("Menu");

	lsmf::Transform transform{};
	transform.SetScale({ 1,1,1 });
	auto font = lsmf::ResourceManager::GetInstance().LoadFont("NesLowercaseRegular-lePq.ttf", 10);

	{
		// fps counter
		auto fps_Object = std::make_unique<lsmf::GameObject>();
		auto fps = std::make_unique<lsmf::FPSComponent>(fps_Object.get(), font);
		fps_Object->AddComponent(std::move(fps));
		sceneMenu->Add(std::move(fps_Object));
	}

	{
		auto menu_Object = std::make_unique<lsmf::GameObject>();
		auto menu = std::make_unique<lsmf::MainMenuComponent>(menu_Object.get());
		lsmf::MainMenuComponent* menuPtr = menu.get();
		menu_Object->AddComponent(std::move(menu));
		sceneMenu->Add(std::move(menu_Object));

		//go = std::make_unique<lsmf::GameObject>();
	//transform.SetPosition({ -60, 0, 10 });
	//go->GetTransform()->SetPosition(transform.GetPosition());
	//go->GetTransform()->SetScale(glm::vec3(0.55f, 0.55f, 1));
	//auto comp = std::make_unique<lsmf::SpriteComponent>(go.get(), "BomberManMenu.jpg");
	//go->AddComponent(std::move(comp));
	//scene->Add(std::move(go));
		transform.SetPosition(20, 100, 3);
		auto button1_Object = std::make_unique<lsmf::GameObject>();
		button1_Object->GetTransform()->SetPosition(transform.GetPosition());
		auto button1 = std::make_unique<lsmf::ButtonComponent>(button1_Object.get(), &LoadSingleplayer, "SinglePlayer", font);
		menuPtr->AddButton(button1.get());
		button1_Object->AddComponent(std::move(button1));
		sceneMenu->Add(std::move(button1_Object));

		transform.SetPosition(120, 100, 3);
		auto button2_Object = std::make_unique<lsmf::GameObject>();
		button2_Object->GetTransform()->SetPosition(transform.GetPosition());
		auto button2 = std::make_unique<lsmf::ButtonComponent>(button2_Object.get(), &LoadCoop, "Coop", font);
		menuPtr->AddButton(button2.get());
		button2_Object->AddComponent(std::move(button2));
		sceneMenu->Add(std::move(button2_Object));

		transform.SetPosition(220, 100, 3);
		auto button3_Object = std::make_unique<lsmf::GameObject>();
		button3_Object->GetTransform()->SetPosition(transform.GetPosition());
		auto button3 = std::make_unique<lsmf::ButtonComponent>(button3_Object.get(), &LoadVersus, "Versus", font);
		menuPtr->AddButton(button3.get());
		button3_Object->AddComponent(std::move(button3));
		sceneMenu->Add(std::move(button3_Object));

	}

	



	


}

int main(int, char*[]) {
	g_engine = new lsmf::Minigin(DATAPATH);
	//g_sound_system = new lsmf::DebugSoundSystem();
	g_sound_system = new lsmf::SDLSoundSystem();

	lsmf::globalSignals::OnPlayerWin.Connect(level::NextLevel);

	g_engine->Run(load);


	lsmf::sound::PlaySoundSignal.DisconnectAll();
	lsmf::sound::PlaySoundSignal.End();

	lsmf::globalSignals::OnEnemyDeath.DisconnectAll();
	lsmf::globalSignals::OnEnemyDeath.End();

	delete g_engine;
	delete g_sound_system;
    return 0;
}