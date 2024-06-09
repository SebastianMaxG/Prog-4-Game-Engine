#include "TileGrid.h"

#include <iostream>
#include <fstream>

#include "TransformComponent.h"

namespace lsmf
{

	TileGrid::TileGrid(GameObject* gameObject, glm::vec2 startPos)
		:BaseComponent(gameObject)
		, m_StartPos(startPos)
	{
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
	void TileGrid::LoadFromFile(const std::string& filename, Scene& scene)
	{
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
				scene.Add(std::move(go));

			}
		}
	}
}
