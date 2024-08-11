#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "Scene.h"
#include "Tile.h"
#include "signal.h"
#include "Enemy.h"

namespace lsmf
{
    class TileGrid : public BaseComponent
	{
    public:
        TileGrid(GameObject* gameObject, glm::vec2 startPos = {});

        Tile* GetTile(int x, int y);  // Returns the tile at the given coordinates
        Tile* GetTile(const glm::vec2& pos, int& x, int& y);  // Returns the tile at the given position
        void LoadFromFile(const std::string& filename, Scene* scene);
        void GenerateRandom(const int level, Scene* scene);

        std::vector<Tile*> GetEmptyTiles() const;

        void SetPlayer(Player* player);

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        void EnemyDeath(Enemy::EnemyType);
        bool EnemiesAlive() const { return (m_NrEnemies > 0); }

        const std::vector<Player*>* GetPlayers() const;

    private:
        std::vector<std::vector<Tile*>> m_Tiles;
        const int TILE_SIZE = 16;
        glm::vec2 m_StartPos{};

        int m_Width{};
        int m_Height{};
        int m_NrEnemies{};

        bool m_Init{ false };

        signal::Connection<Enemy::EnemyType>*  m_EnemyDeathConnection;

        std::vector<Player*> m_Players;
        
    };
}
