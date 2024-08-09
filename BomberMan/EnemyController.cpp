#include "EnemyController.h"

#include "CollisionHandeler.h"
#include "GameObject.h"
#include "Tile.h"
#include "TileGrid.h"
#include "TransformComponent.h"
#include <RandomNumber.h>

namespace lsmf
{
    EnemyController::EnemyController(GameObject* gameObject, TileGrid* tileGrid)
        : ControllerComponent(gameObject), m_TileGrid(tileGrid)
    {
  //      Tile* tile{};
		//while (!tile || tile->GetState() != Tile::TileState::Empty)
		//{
		//	// Initialize the enemy's position to a random tile
		//	const int x = rand() % m_TileGrid->GetWidth();
		//	const int y = rand() % m_TileGrid->GetHeight();
		//	tile = m_TileGrid->GetTile(x, y);
		//}
  //      gameObject->GetTransform()->SetLocalTransform(tile->GetGameObject()->GetTransform()->GetWorldTransform());
        m_MoveDirection = {1,0};
        m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);

        m_CollisionConnection = collision::OnCollide.Connect(this, &EnemyController::CollisionEvent);

    }
	void EnemyController::HandleInput(SDL_Event)
	{
	}
    void EnemyController::CollisionEvent(GameObject* collider, GameObject* other)
    {
        if (GetGameObject() == collider)
        {
            if (Tile* collisionTile = dynamic_cast<Tile*>(other->GetComponent(typeid(Tile))))
            {
	            switch (collisionTile->GetState())
	            {
	            case Tile::TileState::Wall:
	            case Tile::TileState::Crate:
                case Tile::TileState::Bomb:
                {
                    glm::vec2 pos = GetGameObject()->GetTransform()->GetPosition();
                    int x, y;
                    m_TileGrid->GetTile(pos, x, y);

                    // get the tiles around the enemy that are not solid objects
                    std::vector<Tile*> validTiles;
                    if (Tile* tile = m_TileGrid->GetTile(x - 1, y); tile && tile->GetState() != Tile::TileState::Wall && tile->GetState() != Tile::TileState::Crate && tile->GetState() != Tile::TileState::Bomb)
                    {
                        validTiles.push_back(tile);
                    }
                    if (Tile* tile = m_TileGrid->GetTile(x + 1, y); tile && tile->GetState() != Tile::TileState::Wall && tile->GetState() != Tile::TileState::Crate && tile->GetState() != Tile::TileState::Bomb)
                    {
                        validTiles.push_back(tile);
                    }
                    if (m_CanMoveVertical)
                    {
                        if (Tile* tile = m_TileGrid->GetTile(x, y - 1); tile && tile->GetState() != Tile::TileState::Wall && tile->GetState() != Tile::TileState::Crate && tile->GetState() != Tile::TileState::Bomb)
                        {
                            validTiles.push_back(tile);
                        }
                        if (Tile* tile = m_TileGrid->GetTile(x, y + 1); tile && tile->GetState() != Tile::TileState::Wall && tile->GetState() != Tile::TileState::Crate && tile->GetState() != Tile::TileState::Bomb)
                        {
                            validTiles.push_back(tile);
                        }
                    }
                    if (!validTiles.empty())
                    {
                        Tile* newTile = validTiles[random::GetRandomNumber(0, validTiles.size() - 1)];
                        m_MoveDirection = glm::vec2{ newTile->GetGameObject()->GetTransform()->GetPosition() } - pos;
                        // normalize the direction into a cardinal unit vector
                        if (abs(m_MoveDirection.x) > abs(m_MoveDirection.y))
                        {
	                        m_MoveDirection = { m_MoveDirection.x, 0 };
                        }
						else
						{
							m_MoveDirection = { 0, m_MoveDirection.y };
						}
                        m_MoveDirection = glm::normalize(m_MoveDirection);
                        m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
                    }
                    else
                    {
                        // if there are no valid tiles , just move in the opposite direction
                        m_MoveDirection = -m_MoveDirection;
                    }
                    break;
                }
	            default:
                    break;
	            }
            }
		}
    }
    
}

