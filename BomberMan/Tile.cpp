#include "Tile.h"
#include "TileState.h"
#include "CollisionComponent.h"
#include "CollisionHandeler.h"
#include "Enemy.h"
#include "GameObject.h"
#include "GlobalEngineSignals.h"
#include "GlobalSignals.h"
#include "Player.h"
#include "SoundSystem.h"
#include "SpriteComponent.h"
#include "TileGrid.h"
#include "TransformComponent.h"

namespace lsmf
{
    Tile::Tile(GameObject* gameObject, TileType type, bool isExit, PowerUpType power)
        : BaseComponent(gameObject)
        , m_Type(type)
        , m_PowerUpType(power)
        , m_ContainsExit(isExit)
        , m_CurrentState(nullptr)
    {
        auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "TileSprite.png");
        m_SpriteComponent = spriteComponent.get();
        gameObject->AddComponent(std::move(spriteComponent));
        m_SpriteComponent->SetFrames(4, 4, 1, 0.4);
        m_SpriteComponent->SetColumn(static_cast<int>(m_Type));

        spriteComponent = std::make_unique<SpriteComponent>(gameObject, "BombSprites.png");
        m_BombSpriteComponent = spriteComponent.get();
        gameObject->AddComponent(std::move(spriteComponent));
        m_BombSpriteComponent->SetFrames(3, 8, 3, 0.2);
        m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
        m_BombSpriteComponent->Stop();
        m_BombSpriteComponent->SetZ(3);
        m_BombSpriteComponent->SetPingPong(true);

        spriteComponent = std::make_unique<SpriteComponent>(gameObject, "PowerUpSprite.png");
        m_PowerUpSpriteComponent = spriteComponent.get();
        gameObject->AddComponent(std::move(spriteComponent));
        m_PowerUpSpriteComponent->SetFrames(1, 8, 1, 0);
        m_PowerUpSpriteComponent->SetColumn(static_cast<int>(m_PowerUpType));
        m_PowerUpSpriteComponent->Stop();
        m_PowerUpSpriteComponent->SetZ(3);

        auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{ 0,0,0,0 }, true);
        m_CollisionComponent = collisionComponent.get();
        gameObject->AddComponent(std::move(collisionComponent));

        m_CollisionConnection = collision::OnCollide.Connect(this, &Tile::CollisionEvent);

        const glm::vec2 size = m_SpriteComponent->GetTextureSize();
        const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
        m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),static_cast<int>(size.y) });

        switch (m_Type)
        {
        case TileType::Exit:
            ChangeState(new ExitState());
            break;
        case TileType::Crate:
            ChangeState(new CrateState());
            break;
        case TileType::Wall:
            ChangeState(new WallState());
            break;
        case TileType::Empty:
            ChangeState(new EmptyState());
            break;
        }
    }

    Tile::~Tile()
    {
        m_CollisionConnection->Disconnect();
        delete m_CurrentState;
    }

    void Tile::Update(double deltaTime)
    {
        if (m_CurrentState)
        {
            m_CurrentState->Update(this, deltaTime);
        }
        m_PlayerOnTile = nullptr;
        m_EnemyOnTile = nullptr;
    }

    void Tile::BreakCrate()
    {
        if (m_Type == TileType::Crate)
        {
            constexpr double frameTime = 0.4;
            m_SpriteComponent->SetFrames(4, 4, 4, frameTime);
            m_SpriteComponent->SetFrame();
            m_IsCrateBroken = true;
            m_BreakTime = 4 * frameTime;
        }
    }

    void Tile::CollisionEvent(GameObject* collider, GameObject* other)
    {
        if (other == GetGameObject())
        {
            if (Player* player = dynamic_cast<Player*>(collider->GetComponent(typeid(Player))))
            {
                m_PlayerOnTile = player;
            }
            if (Enemy* enemy = dynamic_cast<Enemy*>(collider->GetComponent(typeid(Enemy))))
            {
                m_EnemyOnTile = enemy;
            }
        }
        if (m_CurrentState)
        {
            m_CurrentState->OnCollision(this, collider, other);
        }
    }

    void Tile::SetCrate(PowerUpType powerUp, bool isExit)
    {
        m_ContainsExit = isExit;
        m_PowerUpType = powerUp;
        ChangeState(new CrateState());
    }

    void Tile::ChangeState(TileStateBase* newState)
    {
        if (m_CurrentState)
        {
            delete m_CurrentState;
        }
        m_CurrentState = newState;
        if (m_CurrentState)
        {
            m_CurrentState->Enter(this);
        }
    }

    void Tile::EnterBomb(int bombRange)
    {
        m_BombRange = bombRange;
        ChangeState(new BombState());
    }

    void Tile::EnterExplosion(int range, BombDir dir)
    {
        m_BombRange = range;
        m_BombDir = dir;
        ChangeState(new ExplosionState());
    }
}