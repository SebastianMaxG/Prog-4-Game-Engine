#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "Scene.h"
#include "Tile.h"

namespace lsmf
{

    class TileGrid : public BaseComponent
	{
    public:
        TileGrid(GameObject* gameObject);

        Tile* GetTile(int x, int y);  // Returns the tile at the given coordinates
        Tile* GetTile(const glm::vec2& pos, int& x, int& y);  // Returns the tile at the given position
        void LoadFromFile(const std::string& filename, Scene& scene);

    private:
        std::vector<std::vector<Tile*>> m_Tiles;
        const int TILE_SIZE = 16;
        glm::vec2 m_StartPos{};
        
    };
}
