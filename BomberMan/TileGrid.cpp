#include "TileGrid.h"

namespace lsmf
{

	TileGrid::TileGrid(int width, int height)
	{
		m_Tiles.resize(width);
		for (int i = 0; i < width; ++i)
		{
			m_Tiles[i].resize(height);
		}
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
		x = static_cast<int>(pos.x - m_StartPos.x) / TILE_SIZE;
		y = static_cast<int>(pos.y - m_StartPos.y) / TILE_SIZE;
		if (x < 0 || x >= static_cast<int>(m_Tiles.size()) || y < 0 || y >= static_cast<int>(m_Tiles[0].size()))
		{
			return nullptr;
		}
		return m_Tiles[x][y];
	}
}
