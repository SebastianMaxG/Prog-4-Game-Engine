#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "Tile.h"

namespace lsmf
{

    class TileGrid {
    public:
        TileGrid(int width, int height);

        Tile* GetTile(int x, int y);  // Returns the tile at the given coordinates
        Tile* GetTile(const glm::vec2& pos, int& x, int& y);  // Returns the tile at the given position

    private:
        std::vector<std::vector<Tile*>> m_Tiles;
        const int TILE_SIZE = 16;
        glm::vec2 m_StartPos{};
        
    };
}
