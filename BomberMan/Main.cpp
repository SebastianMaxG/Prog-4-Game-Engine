#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>

#include "CollisionHandeler.h"
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
#include "RandomNumber.h"
#include "ScoreSystem.h"
#include "SoundSystem.h"
#include "Tile.h"
#include "TileGrid.h"


lsmf::Minigin* g_engine;
lsmf::SoundSystem* g_sound_system;
const std::string DATAPATH{ "../Data/" };
void LoadMenu();
namespace level
{
	int g_CurrentLevel{ 1 };

	void ReturnToMenu()
	{
		//delete the current level and return to the menu
		lsmf::SceneManager::GetInstance().DeleteScene("HighScores");
		lsmf::SceneManager::GetInstance().DeleteScene("Level" + std::to_string(g_CurrentLevel));
		g_CurrentLevel = 1; //reset to level 1
		LoadMenu();
	}

	void HighScores()
	{
		lsmf::sound::StopAllSoundsSignal.Emit();
		lsmf::sound::PlaySoundSignal.Emit("End.mp3", 2);
		//create a hightscore component and load the highscores
		auto font = lsmf::ResourceManager::GetInstance().LoadFont("NesLowercaseRegular-lePq.ttf", 15);

		lsmf::Transform transform{};
		transform.SetScale({ 1,1,1 });

		const auto level = lsmf::SceneManager::GetInstance().GetScene("Level" + std::to_string(g_CurrentLevel));
		if (!level)
		{
			std::cout << "Level not found" << std::endl;
			ReturnToMenu();
			return;
		}
		level->SetActive(false);
		if (const auto scoreSystem = level->Get("ScoreSystem"))
		{
			if (lsmf::ScoreSystem* scores = dynamic_cast<lsmf::ScoreSystem*>(scoreSystem->GetComponent(typeid(lsmf::ScoreSystem))))
			{
				transform.SetPosition({ 20, 50, 10 });

				auto sceneScores = lsmf::SceneManager::GetInstance().CreateScene("HighScores");
				
				auto go = std::make_unique<lsmf::GameObject>();
				transform.SetPosition({ transform.GetPosition().x, transform.GetPosition().y, 2 });
				go->GetTransform()->SetPosition(transform.GetPosition());
				go->AddComponent(std::move(std::make_unique<lsmf::HighScoreSystem>(go.get(), scores, DATAPATH + "HighScores.bin",5, font, sceneScores)));
				sceneScores->Add(std::move(go));

				go = std::make_unique<lsmf::GameObject>();
				transform.SetPosition({ 20, 20, 10 });
				go->GetTransform()->SetPosition(transform.GetPosition());
				go->AddComponent(std::move(std::make_unique<lsmf::TextComponent>(go.get(), "Game Over", font)));
				sceneScores->Add(std::move(go));

				go = std::make_unique<lsmf::GameObject>();
				transform.SetPosition({ 110, 50, 10 });
				go->GetTransform()->SetPosition(transform.GetPosition());
				go->AddComponent(std::move(std::make_unique<lsmf::TextComponent>(go.get(), std::to_string(scores->GetScore()), font)));
				sceneScores->Add(std::move(go));
			}
		}
		else
		{
			std::cout << "ScoreSystem not found" << std::endl;
			ReturnToMenu();
		}

	}

	void PlayerDeath()
	{
		// check if there are any players left
		auto level = lsmf::SceneManager::GetInstance().GetScene("Level" + std::to_string(g_CurrentLevel));
		if (!level)
		{
			std::cout << "Level not found" << std::endl;
			ReturnToMenu();
			return;
		}
		auto player1 = level->Get("Player1");
		auto player2 = level->Get("Player2");
		if (!player1 && !player2)
		{
			HighScores();
		}
		bool allPlayersDead = true;
		if (player1)
		{
			if (lsmf::Player* player = dynamic_cast<lsmf::Player*>(player1->GetComponent(typeid(lsmf::Player))))
			{
				if (player->IsAlive())
				{
					allPlayersDead = false;
				}
			}
		}
		if (player2)
		{
			if (lsmf::Player* player = dynamic_cast<lsmf::Player*>(player2->GetComponent(typeid(lsmf::Player))))
			{
				if (player->IsAlive())
				{
					allPlayersDead = false;
				}
			}
		}
		if (allPlayersDead)
		{
			HighScores();
		}

	}

	void NextLevel()
	{
		lsmf::sound::StopAllSoundsSignal.Emit();
		lsmf::sound::PlaySoundSignal.Emit("Game.mp3", 2);

		lsmf::CollisionHandler::GetInstance().ClearQueue();
		if (g_CurrentLevel > 4)
		{
			HighScores();
			return;
		}
		//get the player(s) and the score system and create the next level
		auto level = lsmf::SceneManager::GetInstance().GetScene("Level" + std::to_string(g_CurrentLevel));
		if (!level)
		{
			std::cout << "Level not found" << std::endl;
			ReturnToMenu();
			return;
		}
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
		//lsmf::SceneManager::GetInstance().ClearScenes();
		++g_CurrentLevel;
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
			if (lsmf::Player* player = dynamic_cast<lsmf::Player*>(Player1->GetComponent(typeid(lsmf::Player))))
			{
				player->Reset(tileGrid);
				tileGrid->SetPlayer(player);
			}
			
			level->Add(std::move(Player1));
		}
		if (Player2)
		{
			auto playerStart = tileGrid->GetTile(1, 2)->GetGameObject()->GetTransform()->GetPosition();

			Player2->GetTransform()->SetPosition({ playerStart.x, playerStart.y, 2 });
			if (lsmf::Player* player = dynamic_cast<lsmf::Player*>(Player2->GetComponent(typeid(lsmf::Player))))
			{
				player->Reset(tileGrid);
				tileGrid->SetPlayer(player);
			}
			else if (lsmf::Enemy* enemy = dynamic_cast<lsmf::Enemy*>(Player2->GetComponent(typeid(lsmf::Enemy))))
			{
				enemy->Reset(tileGrid);
			}
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

	lsmf::sound::StopAllSoundsSignal.Emit();
	lsmf::sound::PlaySoundSignal.Emit("Game.mp3", 2);
	lsmf::SceneManager::GetInstance().DeleteScene("Menu");

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
	auto player = std::make_unique<lsmf::Player>(go.get(), tileGrid, 0, true);
	tileGrid->SetPlayer(player.get());
	go->AddComponent(std::move(player));
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

	lsmf::sound::StopAllSoundsSignal.Emit();
	lsmf::sound::PlaySoundSignal.Emit("Game.mp3", 2);
	lsmf::SceneManager::GetInstance().DeleteScene("Menu");


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
	auto player = std::make_unique<lsmf::Player>(go.get(), tileGrid, 1, true);
	tileGrid->SetPlayer(player.get());
	go->AddComponent(std::move(player));
	level->Add(std::move(go));

	playerStart = tileGrid->GetTile(1, 2)->GetGameObject()->GetTransform()->GetPosition();

	go = std::make_unique<lsmf::GameObject>();
	go->SetName("Player2");
	transform.SetPosition({ playerStart.x, playerStart.y, 2 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	player = std::make_unique<lsmf::Player>(go.get(), tileGrid, 0);
	tileGrid->SetPlayer(player.get());
	go->AddComponent(std::move(player));
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

	lsmf::sound::StopAllSoundsSignal.Emit();
	lsmf::sound::PlaySoundSignal.Emit("Game.mp3", 2);
	lsmf::SceneManager::GetInstance().DeleteScene("Menu");

	//todo: load versus (one players, one enemy with player controller)
	std::cout << "Versus" << std::endl;

	const auto level = lsmf::SceneManager::GetInstance().CreateScene("Level" + std::to_string(level::g_CurrentLevel));
	lsmf::Transform transform{};

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
	auto player = std::make_unique<lsmf::Player>(go.get(), tileGrid, 1, true);
	tileGrid->SetPlayer(player.get());
	go->AddComponent(std::move(player));
	level->Add(std::move(go));

	// spawn the 2nd player in a random empty tile

	auto tiles = tileGrid->GetEmptyTiles();
	if (tiles.empty())
	{
		std::cout << "No empty tiles found" << std::endl;
		return;
	}
	size_t index  = lsmf::random::GetRandomNumber(13, tiles.size());
	auto tile = tiles[index];
	playerStart = tile->GetGameObject()->GetTransform()->GetPosition();

	go = std::make_unique<lsmf::GameObject>();
	go->SetName("Player2");
	transform.SetPosition({ playerStart.x, playerStart.y, 2 });
	go->GetTransform()->SetPosition(transform.GetPosition());
	go->AddComponent(std::move(std::make_unique<lsmf::Enemy>(go.get(), 0)));
	level->Add(std::move(go));


	auto go2 = std::make_unique<lsmf::GameObject>();
	go2->SetName("ScoreSystem");
	transform.SetPosition({ 80, 20, 10 });
	go2->GetTransform()->SetPosition(transform.GetPosition());
	go2->AddComponent(std::move(std::make_unique<lsmf::ScoreSystem>(go2.get())));
	level->Add(std::move(go2));
}



void LoadMenu()
{
	int windowX{}, windowY{};
	SDL_GetWindowSize(g_engine->GetSDLWindow(), &windowX, &windowY);
	const auto sceneMenu = lsmf::SceneManager::GetInstance().CreateScene("Menu");

	lsmf::Transform transform{};
	transform.SetScale({ 1,1,1 });
	auto font = lsmf::ResourceManager::GetInstance().LoadFont("NesLowercaseRegular-lePq.ttf", 13);

	//{
	//	// fps counter
	//	auto fps_Object = std::make_unique<lsmf::GameObject>();
	//	auto fps = std::make_unique<lsmf::FPSComponent>(fps_Object.get(), font);
	//	fps_Object->AddComponent(std::move(fps));
	//	sceneMenu->Add(std::move(fps_Object));
	//}

	//play menu.mp3

	lsmf::sound::StopAllSoundsSignal.Emit();
	lsmf::sound::PlaySoundSignal.Emit("Menu.mp3", 2);

	{
		auto menu_Object = std::make_unique<lsmf::GameObject>();
		auto menu = std::make_unique<lsmf::MainMenuComponent>(menu_Object.get());
		lsmf::MainMenuComponent* menuPtr = menu.get();
		menu_Object->AddComponent(std::move(menu));
		sceneMenu->Add(std::move(menu_Object));

		auto go = std::make_unique<lsmf::GameObject>();
		transform.SetPosition({ -60, 0, 0 });
		go->GetTransform()->SetPosition(transform.GetPosition());
		go->GetTransform()->SetScale(glm::vec3(0.55f, 0.55f, 1));
		auto comp = std::make_unique<lsmf::SpriteComponent>(go.get(), "BomberManMenu.jpg");
		go->AddComponent(std::move(comp));
		sceneMenu->Add(std::move(go));

		transform.SetPosition(50, 155, 3);
		auto button1_Object = std::make_unique<lsmf::GameObject>();
		button1_Object->GetTransform()->SetPosition(transform.GetPosition());
		auto button1 = std::make_unique<lsmf::ButtonComponent>(button1_Object.get(), &LoadSingleplayer, "Single", font);
		menuPtr->AddButton(button1.get());
		button1_Object->AddComponent(std::move(button1));
		sceneMenu->Add(std::move(button1_Object));

		transform.SetPosition(130, 155, 3);
		auto button2_Object = std::make_unique<lsmf::GameObject>();
		button2_Object->GetTransform()->SetPosition(transform.GetPosition());
		auto button2 = std::make_unique<lsmf::ButtonComponent>(button2_Object.get(), &LoadCoop, "Coop", font);
		menuPtr->AddButton(button2.get());
		button2_Object->AddComponent(std::move(button2));
		sceneMenu->Add(std::move(button2_Object));

		transform.SetPosition(210, 155, 3);
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
	lsmf::globalSignals::OnPlayerDeath.Connect(level::PlayerDeath);
	lsmf::globalSignals::ReturnToMenu.Connect(level::ReturnToMenu);

	g_engine->Run(LoadMenu);


	lsmf::sound::PlaySoundSignal.DisconnectAll();
	lsmf::sound::PlaySoundSignal.End();

	lsmf::globalSignals::OnEnemyDeath.DisconnectAll();
	lsmf::globalSignals::OnEnemyDeath.End();

	lsmf::SceneManager::GetInstance().ClearScenes();
	delete g_engine;
	delete g_sound_system;
    return 0;
}