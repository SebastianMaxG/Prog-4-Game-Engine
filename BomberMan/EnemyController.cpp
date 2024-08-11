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
        m_MoveDirection = { 1, 0 };
        m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
        m_CollisionConnection = collision::OnCollide.Connect(this, &EnemyController::CollisionEvent);
        int x, y;
        m_CurrentTile = m_TileGrid->GetTile(gameObject->GetTransform()->GetPosition(), x, y);
    }

    EnemyController::~EnemyController()
    {
        m_CollisionConnection->Disconnect();
    }

    void EnemyController::Update(double deltaTime)
    {
        switch (m_CurrentState)
        {
        case State::Idle:
            UpdateIdleState(deltaTime);
            break;
        case State::Moving:
            UpdateMovingState(deltaTime);
            break;
        case State::Chasing:
            UpdateChasingState(deltaTime);
            break;
        case State::Colliding:
            UpdateCollidingState(deltaTime);
            break;
        }
    }

    void EnemyController::UpdateIdleState(double)
    {
        // Try to move again
        m_CurrentState = State::Moving;
    }

    void EnemyController::UpdateMovingState(double)
    {
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
                m_CurrentState = State::Chasing;
            }
            else
            {
                if (random::GetRandomNumber(0, 100) < 5)
                {
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
                        m_CurrentState = State::Idle;
                    }
                }
            }
        }
    }

    void EnemyController::UpdateChasingState(double)
    {
        const glm::vec3 posRaw = GetGameObject()->GetTransform()->GetPosition();
        for (Player* player : *m_TileGrid->GetPlayers())
        {
            glm::vec2 playerPos = { player->GetGameObject()->GetTransform()->GetPosition() };
            if (abs(m_MoveDirection.y) > 0)
            {
                if (abs(playerPos.y - posRaw.y) <= 4)
                {
                    if (playerPos.x < posRaw.x)
                    {
                        m_MoveDirection = { -1, 0 };
                        m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
                        return;
                    }
                    if (playerPos.x > posRaw.x)
                    {
                        m_MoveDirection = { 1, 0 };
                        m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
                        return;
                    }
                }
            }
            else if (abs(playerPos.x - posRaw.x) <= 4)
            {
                if (playerPos.y < posRaw.y)
                {
                    m_MoveDirection = { 0, -1 };
                    m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
                    return;
                }
                if (playerPos.y > posRaw.y)
                {
                    m_MoveDirection = { 0, 1 };
                    m_MoveTransform.SetPosition(m_MoveDirection.x, m_MoveDirection.y, 0);
                    return;
                }
            }
        }
        m_CurrentState = State::Moving;
    }

    void EnemyController::UpdateCollidingState(double)
    {
        const glm::vec2 pos = { GetGameObject()->GetTransform()->GetPosition() };
        int x, y;
        m_TileGrid->GetTile(pos, x, y);
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
            m_CurrentState = State::Moving;
        }
        else
        {
            m_CurrentState = State::Idle;
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
                    m_CurrentState = State::Colliding;
                    break;
                default:
                    break;
                }
            }
        }
    }
}
