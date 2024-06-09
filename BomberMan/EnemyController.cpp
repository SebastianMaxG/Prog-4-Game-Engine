#include "EnemyController.h"

#include "GameObject.h"
#include "Tile.h"
#include "TileGrid.h"
#include "TransformComponent.h"

namespace lsmf
{
    EnemyController::EnemyController(GameObject* gameObject, TileGrid* tileGrid)
        : ControllerComponent(gameObject), m_TileGrid(tileGrid)
    {
        Tile* tile{};
		while (!tile || tile->GetState() == Tile::TileState::Wall || tile->GetState() == Tile::TileState::Crate || tile->GetState() == Tile::TileState::Bomb)
		{
			// Initialize the enemy's position to a random tile
			const int x = rand() % m_TileGrid->GetWidth();
			const int y = rand() % m_TileGrid->GetHeight();
			tile = m_TileGrid->GetTile(x, y);
		}
        gameObject->GetTransform()->SetLocalTransform(tile->GetGameObject()->GetTransform()->GetWorldTransform());
		targetPosition = gameObject->GetTransform()->GetPosition();
    }
    void EnemyController::Update(double deltaTime)
    {
        timeSinceLastUpdate += static_cast<float>(deltaTime);
        if (timeSinceLastUpdate >= updateInterval)
        {
            int counter{};
            Tile* newTile{};
            while (!newTile || newTile->GetState() == Tile::TileState::Wall || newTile->GetState() == Tile::TileState::Crate || newTile->GetState() == Tile::TileState::Bomb)
            {
                // Randomly choose a direction to move in
                const int direction = rand() % 4;
                glm::vec2 offset{};
                switch (direction)
                {
                case 0: // up
                    offset = glm::vec2(0, -1);
                    break;
                case 1: // down
                    offset = glm::vec2(0, 1);
                    break;
                case 2: // left
                    offset = glm::vec2(-1, 0);
                    m_Left = true;
                    break;
                case 3: // right
                    offset = glm::vec2(1, 0);
                    m_Left = false;
                    break;
                }

                // Get the current tile
                glm::vec2 pos = GetGameObject()->GetTransform()->GetPosition();
                int x, y;
                m_TileGrid->GetTile(pos, x, y);

                // Calculate the new tile coordinates
                const int newX = x + static_cast<int>(offset.x);
                const int newY = y + static_cast<int>(offset.y);

                // Check if the new tile is valid and not a wall
                newTile = m_TileGrid->GetTile(newX, newY);
                if (counter++ > 5)
                {
                    break;
				}
            }
            if (newTile && newTile->GetState() != Tile::TileState::Wall && newTile->GetState() != Tile::TileState::Crate && newTile->GetState() != Tile::TileState::Bomb)
            {
                // Update the target position
                targetPosition = newTile->GetGameObject()->GetTransform()->GetPosition();
            }

            timeSinceLastUpdate = 0.0f;
        }

        // Interpolate the enemy's position towards the target position
        glm::vec2 currentPosition = GetGameObject()->GetTransform()->GetPosition();
        glm::vec2 newPosition = currentPosition;

        // Only change x or y direction at a time
        if (std::abs(targetPosition.x - currentPosition.x) > std::abs(targetPosition.y - currentPosition.y))
        {
            newPosition.x = glm::mix(currentPosition.x, targetPosition.x, static_cast<float>(deltaTime));
        }
        else
        {
            newPosition.y = glm::mix(currentPosition.y, targetPosition.y, static_cast<float>(deltaTime));
        }

        GetGameObject()->GetTransform()->SetPosition({ newPosition.x,newPosition.y,GetGameObject()->GetTransform()->GetPosition().z });
    }
	void EnemyController::HandleInput(SDL_Event)
	{
	}
}

