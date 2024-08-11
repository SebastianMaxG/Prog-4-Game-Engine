#pragma once
#include "ControllerComponent.h"
#include <random>

#include "Tile.h"

namespace lsmf
{
	class TileGrid;

    class EnemyController : public ControllerComponent
    {
    public:
        enum class Direction
        {
            Up,
            Down,
            Left,
            Right
        };


        EnemyController(GameObject* gameObjectPtr, TileGrid* tileGrid);
        ~EnemyController() override;

        EnemyController(const EnemyController& other) = delete;
        EnemyController(EnemyController&& other) = delete;
        EnemyController& operator=(const EnemyController& other) = delete;
        const EnemyController& operator=(EnemyController&& other) = delete;

        void Update(double deltaTime) override;

        void HandleInput(SDL_Event event) override;

        void LockVerticalMovement() { m_CanMoveVertical = false; }
        void Smart() { m_Smart = true; }


        void CollisionEvent(GameObject* collider, GameObject* other);

    private:

        TileGrid* m_TileGrid;
        bool m_CanMoveVertical = true;
        bool m_Smart {false};
        glm::vec2 m_MoveDirection;

        signal::Connection<GameObject*, GameObject*>* m_CollisionConnection;

        Tile* m_CurrentTile{};
        std::vector<Player*> m_Players;

    };
}
