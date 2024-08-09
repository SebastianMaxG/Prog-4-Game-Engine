#include "TileGrid.h"

#include <iostream>
#include <fstream>
#include <RandomNumber.h>

#include "TransformComponent.h"
#include "GlobalSignals.h"

namespace lsmf
{

	TileGrid::TileGrid(GameObject* gameObject, glm::vec2 startPos)
		:BaseComponent(gameObject)
		, m_StartPos(startPos)
	{
		m_EnemyDeathConnection = globalSignals::OnEnemyDeath.Connect(this, &TileGrid::EnemyDeath);
	}

	Tile* TileGrid::GetTile(int x, int y)
	{
		if (x < 0 || x >= static_cast<int>(m_Tiles.size()) || y < 0 || y >= static_cast<int>(m_Tiles[0].size()))
		{
			return nullptr;
		}
		return m_Tiles[x][y];
	}

	Tile* TileGrid::GetTile(const glm::vec2& pos, int& x, int& y)
	{
		x = static_cast<int>(pos.x + static_cast<float>(TILE_SIZE) / 2.f - m_StartPos.x) / TILE_SIZE;
		y = static_cast<int>(pos.y + static_cast<float>(TILE_SIZE) / 2.f - m_StartPos.y) / TILE_SIZE;
		if (x < 0 || x >= static_cast<int>(m_Tiles.size()) || y < 0 || y >= static_cast<int>(m_Tiles[0].size()))
		{
			return nullptr;
		}
		return m_Tiles[x][y];
	}
	void TileGrid::LoadFromFile(const std::string& filename, Scene* scene)
	{
		if (m_Init)
		{
			std::cerr << "TileGrid already initialized\n";
			return;
		}
		std::ifstream file(filename);
		if (!file)
		{
			std::cerr << "Failed to open file: " << filename << std::endl;
			return;
		}

		int width, height;
		file >> width >> height;
		m_Width = width;
		m_Height = height;
		m_Tiles.resize(width);
		for (int x = 0; x < width; ++x)
		{
			m_Tiles[x].resize(height);
			for (int y = 0; y < height; ++y)
			{
				int tileType;
				file >> tileType;
				bool isExit;
				file >> isExit;
				int powerUpType;
				file >> powerUpType;


				auto go = std::make_unique<lsmf::GameObject>();
				Transform transform{};
				transform.SetPosition({ m_StartPos.x + (x * TILE_SIZE),m_StartPos.y + (y * TILE_SIZE), 0 });
				go->GetTransform()->SetPosition(transform.GetPosition());
				auto tile = std::make_unique<lsmf::Tile>(go.get(),static_cast<Tile::TileType>(tileType), isExit, static_cast<Tile::PowerUpType>(powerUpType));

				m_Tiles[x][y] = tile.get();
				tile->SetGridPos(x, y, this);

				go->AddComponent(std::move(tile));
				scene->Add(std::move(go));

			}
		}
		m_Init = true;
	}

	void TileGrid::GenerateRandom(const int level, Scene* scene)
	{
		if (m_Init)
		{
			std::cerr << "TileGrid already initialized\n";
			return;
		}
		int x = 21;
		int y = 13;

		m_Tiles.resize(x);
		//create border tiles around the level
		for (int i = 0; i < x; ++i)
		{
			m_Tiles[i].resize(y);
			auto go = std::make_unique<lsmf::GameObject>();
			Transform transform{};
			transform.SetPosition({ m_StartPos.x + (i * TILE_SIZE),m_StartPos.y, 0 });
			go->GetTransform()->SetPosition(transform.GetPosition());
			auto tile = std::make_unique<lsmf::Tile>(go.get(), Tile::TileType::Wall, false, Tile::PowerUpType::None);
			m_Tiles[i][0] = (tile.get());
			tile->SetGridPos(i, 0, this);
			go->AddComponent(std::move(tile));
			scene->Add(std::move(go));
		}
		for (int i = 1; i < y - 1; ++i)
		{
			auto go = std::make_unique<lsmf::GameObject>();
			Transform transform{};
			transform.SetPosition({ m_StartPos.x + ((x - 1) * TILE_SIZE),m_StartPos.y + (i * TILE_SIZE), 0 });
			go->GetTransform()->SetPosition(transform.GetPosition());
			auto tile = std::make_unique<lsmf::Tile>(go.get(), Tile::TileType::Wall, false, Tile::PowerUpType::None);
			m_Tiles[x - 1][i] = (tile.get());
			tile->SetGridPos(x, i, this);
			go->AddComponent(std::move(tile));
			scene->Add(std::move(go));
		}
		for (int i = 0; i < x; ++i)
		{
			auto go = std::make_unique<lsmf::GameObject>();
			Transform transform{};
			transform.SetPosition({ m_StartPos.x + (i * TILE_SIZE),m_StartPos.y + ((y - 1) * TILE_SIZE), 0 });
			go->GetTransform()->SetPosition(transform.GetPosition());
			auto tile = std::make_unique<lsmf::Tile>(go.get(), Tile::TileType::Wall, false, Tile::PowerUpType::None);
			m_Tiles[i][y - 1] = (tile.get());
			tile->SetGridPos(i, y - 1, this);
			go->AddComponent(std::move(tile));
			scene->Add(std::move(go));
		}
		for (int i = 1; i < y - 1; ++i)
		{
			auto go = std::make_unique<lsmf::GameObject>();
			Transform transform{};
			transform.SetPosition({ m_StartPos.x,m_StartPos.y + (i * TILE_SIZE), 0 });
			go->GetTransform()->SetPosition(transform.GetPosition());
			auto tile = std::make_unique<lsmf::Tile>(go.get(), Tile::TileType::Wall, false, Tile::PowerUpType::None);
			m_Tiles[0][i] = (tile.get());
			tile->SetGridPos(0, i, this);
			go->AddComponent(std::move(tile));
			scene->Add(std::move(go));
		}

		// fill the rest with empty tiles and every other row create a wall every other tile
		for (int i = 1; i < x - 1; ++i)
		{
			for (int j = 1; j < y - 1; ++j)
			{
				auto go = std::make_unique<lsmf::GameObject>();
				Transform transform{};
				transform.SetPosition({ m_StartPos.x + (i * TILE_SIZE),m_StartPos.y + (j * TILE_SIZE), 0 });
				go->GetTransform()->SetPosition(transform.GetPosition());
				std::unique_ptr<Tile> tile;
				if (i%2 or j%2)
				{
					tile = std::make_unique<lsmf::Tile>(go.get(), Tile::TileType::Empty, false, Tile::PowerUpType::None);
				}
				else
				{
					tile = std::make_unique<lsmf::Tile>(go.get(), Tile::TileType::Wall, false, Tile::PowerUpType::None);
				}
				m_Tiles[i][j] = (tile.get());
				tile->SetGridPos(i, j, this);
				go->AddComponent(std::move(tile));
				scene->Add(std::move(go));
			}
		}

		m_Width = x;
		m_Height = y;




		// Depending on the level spawn a different amount of enemies and crates , always 1 crate with an exit and chances for powerups

		//bool bonusLevel = false;
		std::vector<Enemy::EnemyType> enemies;
		std::vector<Tile::PowerUpType> powerUps;

		switch (level)
		{
		case 1:
			m_NrEnemies = 6;
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);

			powerUps.emplace_back(Tile::PowerUpType::Range);
			break;
		case 2:
			m_NrEnemies = 6;
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);

			enemies.emplace_back(Enemy::EnemyType::Oneal);
			enemies.emplace_back(Enemy::EnemyType::Oneal);
			enemies.emplace_back(Enemy::EnemyType::Oneal);

			powerUps.emplace_back(Tile::PowerUpType::Bomb);

			break;
		case 3:
			m_NrEnemies = 6;
			enemies.emplace_back(Enemy::EnemyType::Balloom);
			enemies.emplace_back(Enemy::EnemyType::Balloom);

			enemies.emplace_back(Enemy::EnemyType::Oneal);
			enemies.emplace_back(Enemy::EnemyType::Oneal);

			enemies.emplace_back(Enemy::EnemyType::Doll);
			enemies.emplace_back(Enemy::EnemyType::Doll);

			powerUps.emplace_back(Tile::PowerUpType::Remote);

			break;
		case 4:
			m_NrEnemies = 6;
			enemies.emplace_back(Enemy::EnemyType::Balloom);

			enemies.emplace_back(Enemy::EnemyType::Oneal);

			enemies.emplace_back(Enemy::EnemyType::Doll);
			enemies.emplace_back(Enemy::EnemyType::Doll);

			enemies.emplace_back(Enemy::EnemyType::Minvo);
			enemies.emplace_back(Enemy::EnemyType::Minvo);

			powerUps.emplace_back(Tile::PowerUpType::Speed);

			break;
		case 5:
			m_NrEnemies = 7;
			enemies.emplace_back(Enemy::EnemyType::Oneal);
			enemies.emplace_back(Enemy::EnemyType::Oneal);
			enemies.emplace_back(Enemy::EnemyType::Oneal);
			enemies.emplace_back(Enemy::EnemyType::Oneal);

			enemies.emplace_back(Enemy::EnemyType::Doll);
			enemies.emplace_back(Enemy::EnemyType::Doll);
			enemies.emplace_back(Enemy::EnemyType::Doll);

			powerUps.emplace_back(Tile::PowerUpType::Bomb);

			break;
		default:
			std::cerr << "Level not implemented\n";
			break;
		}

		auto emptyTiles = GetEmptyTiles();

		// spawn the crates on about 20 procent of the tiles ignoring the first 2 tiles
		int nrCrates = static_cast<int>(static_cast<float>(emptyTiles.size()) * 0.2f);
		for (int i = 0; i < nrCrates; ++i)
		{

			size_t index = random::GetRandomNumber(3, emptyTiles.size() - 1);
			auto tile = emptyTiles[index];
			tile->SetCrate();
			emptyTiles.clear();
			emptyTiles = GetEmptyTiles();
		}
		// for each powerup spawn a crate
		for (auto powerUp : powerUps)
		{
			size_t index = random::GetRandomNumber(3, emptyTiles.size() - 1);
			auto tile = emptyTiles[index];
			tile->SetCrate(powerUp);
			emptyTiles.clear();
			emptyTiles = GetEmptyTiles();
		}
		//spawn the end
		{
			size_t index = random::GetRandomNumber(3, emptyTiles.size() - 1);
			auto tile = emptyTiles[index];
			tile->SetCrate(Tile::PowerUpType::None, true);
			emptyTiles.clear();
			emptyTiles = GetEmptyTiles();
		}

		// spawn enemies
		for (int i = 0; i < m_NrEnemies; ++i)
		{
			size_t index = random::GetRandomNumber(10, emptyTiles.size() - 1);
			auto tile = emptyTiles[index];;
			auto pos = tile->GetGameObject()->GetTransform();

			// make enemy game objects using the pos
			auto go = std::make_unique<lsmf::GameObject>();
			go->GetTransform()->SetPosition(pos->GetPosition());
			go->AddComponent(std::make_unique<lsmf::Enemy>(go.get(), enemies[i], this));
			scene->Add(std::move(go));

		}



		m_Init = true;


		/*
		 * CREDIT : https://gamefaqs.gamespot.com/nes/563390-bomberman/faqs/14622
		 *
		 * Every stage has a 200 second time limit, and every bonus stage has a 30 second
		 * time limit.
		 * 
		 * 
		 * 01 [6 Ballooms / Fire]
		 * 
		 * 02 [3 Ballooms, 3 O'Neals / Bomb]
		 * 
		 * 03 [2 Ballooms, 2 O'Neals, 2 Dolls / Detonator]
		 * 
		 * 04 [1 Balloom, 1 O'Neal, 2 Dolls, 2 Minvos / Speedup]
		 * 
		 * 05 [4 O'Neals, 3 Dolls / Bomb]
		 * 
		 * BONUS STAGE [Ballooms]
		 * 
		 * 06 [2 O'Neals, 3 Dolls, 2 Minvos / Bomb]
		 * 
		 * 07 [2 O'Neals, 3 Dolls, 2 Ovapes / Fire]
		 * 
		 * 08 [1 O'Neal, 2 Dolls, 4 Minvos / Detonator]
		 * 
		 * 09 [1 O'Neal, 1 Doll, 4 Minvos, 1 Doria / Bomb-Walk]
		 * 
		 * 10 [1 O'Neal, 1 Doll, 1 Minvo, 1 Ovape, 3 Dorias / Wall-Walker]
		 * 
		 * BONUS STAGE [O'Neals]
		 * 
		 * 11 [1 O'Neal, 2 Dolls, 3 Minvos, 1 Ovape, 1 Doria / Bomb]
		 * 
		 * 12 [1 O'Neal, 1 Doll, 1 Minvo, 1 Ovape, 4 Dorias / Bomb]
		 * 
		 * 13 [3 Dolls, 3 Minvos, 2 Dorias / Detonator]
		 * 
		 * 14 [7 Ovapes, 1 Pass / Bomb-Walk]
		 * 
		 * 15 [1 Doll, 3 Minvos, 3 Dorias, 1 Pass / Fire]
		 * 
		 * BONUS STAGE [Dolls]
		 * 
		 * 16 [3 Minvos, 4 Dorias, 1 Pass / Wall-Walk]
		 * 
		 * 17 [5 Dolls, 2 Dorias, 1 Pass / Bomb]
		 * 
		 * 18 [3 Ballooms, 3 O'Neals, 2 Passes / Bomb-Walk]
		 * 
		 * 19 [1 Balloom, 1 Ovape, 1 O'Neal, 3 Dolls, 2 Passes / Bomb]
		 * 
		 * 20 [1 O'Neal, 1 Doll, 1 Minvo, 1 Ovape, 2 Dorias, 2 Passes / Detonator]
		 * 
		 * BONUS STAGE [Minvos]
		 * 
		 * 21 [3 Ovapes, 4 Dorias, 2 Passes / Bomb-Walk]
		 * 
		 * 22 [4 Dolls, 3 Minvos, 1 Doria, 1 Pass / Detonator]
		 * 
		 * 23 [2 Dolls, 2 Minvos, 2 Ovapes, 2 Dorias, 1 Pass / Bomb]
		 * 
		 * 24 [1 Doll, 1 Minvo, 2 Ovapes, 4 Dorias, 1 Pass / Detonator]
		 * 
		 * 25 [2 O'Neals, 1 Doll, 1 Minvo, 2 Ovapes, 2 Dorias, 1 Pass / Bomb-Walk]
		 * 
		 * BONUS STAGE [Ovapes]
		 * 
		 * 26 [1 Balloom, 1 O'Neal, 1 Doll, 1 Minvo, 1 Ovape, 2 Dorias, 1 Pass / 
		 * Mystery]
		 * 
		 * 27 [1 Balloom, 1 O'Neal, 1 Ovape, 5 Dorias, 1 Pass / Fire]
		 * 
		 * 28 [1 O'Neal, 3 Dolls, 3 Minvos, 1 Doria, 1 Pass / Bomb]
		 * 
		 * 29 [5 Ovapes, 2 Dorias, 2 Passes / Detonator]
		 * 
		 * 30 [3 Dolls, 2 Minvos, 2 Ovapes, 1 Doria, 1 Pass / Flame-Proof]
		 * 
		 * BONUS STAGE [Dorias]
		 * 
		 * 31 [2 O'Neals, 2 Dolls, 2 Minvos, 2 Ovapes, 2 Dorias / Wall-Walker]
		 * 
		 * 32 [1 O'Neal, 1 Doll, 3 Minvos, 4 Dorias, 1 Pass / Bomb]
		 * 
		 * 33 [2 Dolls, 2 Minvos, 1 Ovape, 3 Dorias, 2 Passes / Detonator]
		 * 
		 * 34 [2 Dolls, 3 Minvos, 3 Dorias, 2 Passes / Mystery]
		 * 
		 * 35 [2 Dolls, 1 Minvo, 1 Ovape, 3 Dorias, 2 Passes / Bomb-Walk]
		 * 
		 * BONUS STAGE [Passes]
		 * 
		 * 36 [2 Dolls, 2 Minvos, 3 Dorias, 3 Passes / Flame-Proof]
		 * 
		 * 37 [2 Dolls, 1 Minvo, 1 Ovape, 3 Dorias, 3 Passes / Detonator]
		 * 
		 * 38 [2 Dolls, 2 Minvos, 3 Dorias, 3 Passes / Fire]
		 * 
		 * 39 [1 Doll, 1 Minvo, 2 Ovapes, 2 Dorias, 4 Passes / Wall-Walker]
		 * 
		 * BONUS STAGE [Pontans]
		 * 
		 * 40 [1 Doll, 2 Minvos, 3 Dorias, 4 Passes / Mystery]
		 * 
		 * 41 [1 Doll, 1 Minvo, 1 Ovape, 3 Dorias, 4 Passes / Detonator]
		 * 
		 * 42 [1 Minvo, 1 Ovape, 3 Dorias, 5 Passes / Wall-Walker]
		 * 
		 * 43 [1 Minvo, 1 Ovape, 2 Dorias, 6 Passes / Bomb-Walk]
		 * 
		 * 44 [1 Minvo, 1 Ovape, 2 Dorias, 6 Passes / Detonator]
		 * 
		 * BONUS STAGE [Pontans]
		 * 
		 * 45 [2 Ovapes, 2 Dorias, 6 Passes / Mystery]
		 * 
		 * 46 [2 Ovapes, 2 Dorias, 6 Passes / Wall-Walker]
		 * 
		 * 47 [2 Ovapes, 2 Dorias, 6 Passes / Bomb-Walk]
		 * 
		 * 48 [1 Ovape, 2 Dorias, 6 Passes, 1 Pontan / Detonator]
		 * 
		 * 49 [2 Ovapes, 1 Doria, 6 Passes, 1 Pontan / Fire-Proof]
		 * 
		 * BONUS STAGE [Pontans]
		 * 
		 * 50 [2 Ovapes, 1 Doria, 5 Passes, 2 Pontans / Mystery]
		 */

	}
	std::vector<Tile*> TileGrid::GetEmptyTiles() const
	{
		std::vector<Tile*> emptyTiles;
		for (auto& row : m_Tiles)
		{
			for (auto tile : row)
			{
				if (tile->GetState() == Tile::TileState::Empty)
				{
					emptyTiles.push_back(tile);
				}
			}
		}
		return emptyTiles;
	}
	void TileGrid::EnemyDeath(Enemy::EnemyType)
	{
		m_NrEnemies--;
	}
}
