#pragma once
#include "ControllerComponent.h"
#include <random>

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


        EnemyController(GameObject* m_GameObjectPtr, TileGrid* tileGrid);
        ~EnemyController() override;

        EnemyController(const EnemyController& other) = delete;
        EnemyController(EnemyController&& other) = delete;
        EnemyController& operator=(const EnemyController& other) = delete;
        const EnemyController& operator=(EnemyController&& other) = delete;

        void HandleInput(SDL_Event event) override;

        bool GetLeft() const { return m_Left; }
        void LockVerticalMovement() { m_CanMoveVertical = false; }


        void CollisionEvent(GameObject* collider, GameObject* other);

    private:

        TileGrid* m_TileGrid;
        bool m_Left = true;
        bool m_CanMoveVertical = true;
        glm::vec2 m_MoveDirection;

        signal::Connection<GameObject*, GameObject*>* m_CollisionConnection;

    };
}
