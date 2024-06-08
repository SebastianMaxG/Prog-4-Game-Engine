#pragma once
#include <vector>
#include "Tile.h"

namespace lsmf
{

    class TileGrid {
    public:
        TileGrid(int width, int height);

        Tile* GetTile(int x, int y);  // Returns the tile at the given coordinates

    private:
        std::vector<std::vector<Tile*>> m_Tiles;
    };
}
