#pragma once
#include "ControllerComponent.h"
namespace lsmf
{
	class TileGrid;

    class EnemyController : public ControllerComponent
    {
    public:
        EnemyController(GameObject* m_GameObjectPtr, TileGrid* tileGrid);
        ~EnemyController() override = default;

        EnemyController(const EnemyController& other) = delete;
        EnemyController(EnemyController&& other) = delete;
        EnemyController& operator=(const EnemyController& other) = delete;
        const EnemyController& operator=(EnemyController&& other) = delete;

        void Update(double deltaTime) override;
        void HandleInput(SDL_Event event) override;

        bool GetLeft() const { return m_Left; }
        void SetUpdateSpeed (float speed) { updateInterval = 1.f/speed; }
        void LockVerticalMovement() { m_CanMoveVertical = false; }
    private:

        TileGrid* m_TileGrid;
        bool m_Left = true;
        glm::vec2 targetPosition;
        float updateInterval = 1.0f; // update target position every 1 second
        float timeSinceLastUpdate = 0.0f;
        float m_CanMoveVertical = true;
    };
}
