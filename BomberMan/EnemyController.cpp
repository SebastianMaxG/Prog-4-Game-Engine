#include "EnemyController.h"

#include "CollisionHandeler.h"
#include "GameObject.h"
#include "Tile.h"
#include "TileGrid.h"
#include "TransformComponent.h"
#include <RandomNumber.h>

#include "Player.h"

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
        int x, y;
        m_CurrentTile = m_TileGrid->GetTile(gameObject->GetTransform()->GetPosition(), x, y);

    }

    EnemyController::~EnemyController()
    {
		m_CollisionConnection->Disconnect();
    }

    void EnemyController::Update(double)
    {
        if (m_MoveDirection == glm::vec2{ 0, 0 })
		{
            m_MoveDirection = { 1, 0 };
            m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
		}
        const glm::vec3 posRaw = GetGameObject()->GetTransform()->GetPosition();
        glm::vec2 pos = { posRaw.x, posRaw.y };
        pos.x -= (m_MoveDirection.x) * 8;
        pos.y -= (m_MoveDirection.y) * 8;
        int x, y;
        Tile* currentTile = m_TileGrid->GetTile(pos, x, y);
        if (m_CurrentTile != currentTile)
        {
            m_CurrentTile = currentTile;

            if (m_Smart)
            {
	            for (Player* player : *m_TileGrid->GetPlayers())
	            {
                    glm::vec2 playerPos = { player->GetGameObject()->GetTransform()->GetPosition() };
	            	if (abs(m_MoveDirection.y) > 0 )
	            	{
	            		if (abs(playerPos.y - posRaw.y) <= 4)
	            		{
	            			if (playerPos.x < posRaw.x)
	            			{
	            				m_MoveDirection = { -1, 0 }; // Move left
	            				m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
	            				return;
	            			}
	            			if (playerPos.x > posRaw.x)
	            			{
	            				m_MoveDirection = { 1, 0 }; // Move right
	            				m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
	            				return;

	            			}
	            		}
	            	}
	            	else if (abs(playerPos.x - posRaw.x) <= 4)
	            	{
	            		if (playerPos.y < posRaw.y)
	            		{
	            			m_MoveDirection = { 0, -1 }; // Move up
                            m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
	            			return;
	            		}
	            		if (playerPos.y > posRaw.y)
	            		{
	            			m_MoveDirection = { 0, 1 }; // Move down
                            m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
	            			return;
	            		}
	            	}
	            }
            }
            else
            {
                // chance to change direction
                if (random::GetRandomNumber(0, 100) < 5)
                {
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
                        // if there are no valid tiles, just move in the opposite direction
                        m_MoveDirection = -m_MoveDirection;
                    }
                }
            }
        }
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
                    const glm::vec2  pos = { GetGameObject()->GetTransform()->GetPosition() };
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

