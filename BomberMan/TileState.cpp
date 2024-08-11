#include "TileState.h"

#include "CollisionComponent.h"
#include "Tile.h"
#include "SoundSystem.h"
#include "GlobalSignals.h"
#include "SpriteComponent.h"
#include "TileGrid.h"

namespace lsmf
{
    TileState ExitState::Enter(Tile* tile)
    {
        tile->m_Type = Tile::TileType::Exit;
        tile->m_SpriteComponent->SetColumn(static_cast<int>(tile->m_Type));
        tile->m_BombSpriteComponent->Stop();
        tile->m_PowerUpSpriteComponent->Stop();
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->AddCollidingChannel(CollisionChannel::Exit);
        tile->m_CollisionComponent->Start();

        return TileState::Exit;
    }

    void ExitState::Update(Tile* tile, double )
    {
        if (tile->m_PlayerOnTile)
        {
            if (tile->m_Grid->EnemiesAlive())
            {
                return;
            }
            sound::PlaySoundSignal.Emit("Bomberman SFX (6).wav", 5);
            globalSignals::OnPlayerWin.Emit();
        }
    }

    void ExitState::OnCollision(Tile* , GameObject* , GameObject* ) {}

    TileState CrateState::Enter(Tile* tile)
    {
        tile->m_Type = Tile::TileType::Crate;
        tile->m_SpriteComponent->SetColumn(static_cast<int>(tile->m_Type));
        tile->m_BombSpriteComponent->Stop();
        tile->m_PowerUpSpriteComponent->Stop();
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->AddCollidingChannel(CollisionChannel::Crate);
        tile->m_CollisionComponent->Start();

        return TileState::Crate;
    }

    void CrateState::Update(Tile* tile, double deltaTime)
    {
        if (tile->m_IsCrateBroken)
        {
            tile->m_BreakTime -= deltaTime;
            if (tile->m_BreakTime <= 0)
            {
                tile->m_IsCrateBroken = false;
                tile->m_SpriteComponent->SetFrame();
                tile->m_SpriteComponent->SetFrames(4, 4, 1, 0);
                if (tile->m_PowerUpType == Tile::PowerUpType::None)
                {
                    if (tile->m_ContainsExit)
                    {
                        tile->ChangeState(new ExitState());
                    }
                    else
                    {
                        tile->ChangeState(new EmptyState());
                    }
                }
                else
                {
                    tile->ChangeState(new PowerUpState());
                }
            }
        }
    }

    void CrateState::OnCollision(Tile* , GameObject* , GameObject* ) {}

    TileState WallState::Enter(Tile* tile)
    {
        tile->m_Type = Tile::TileType::Wall;
        tile->m_SpriteComponent->SetColumn(static_cast<int>(tile->m_Type));
        tile->m_BombSpriteComponent->Stop();
        tile->m_PowerUpSpriteComponent->Stop();
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->AddCollidingChannel(CollisionChannel::Wall);
        tile->m_CollisionComponent->Start();

        return TileState::Wall;
    }

    void WallState::Update(Tile* , double ) {}

    void WallState::OnCollision(Tile* , GameObject* , GameObject* ) {}

    TileState EmptyState::Enter(Tile* tile)
    {
        tile->m_Type = Tile::TileType::Empty;
        tile->m_SpriteComponent->SetColumn(static_cast<int>(tile->m_Type));
        tile->m_BombSpriteComponent->Stop();
        tile->m_PowerUpSpriteComponent->Stop();
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->Stop();

        return TileState::Empty;
    }

    void EmptyState::Update(Tile* , double ) {}

    void EmptyState::OnCollision(Tile* , GameObject* , GameObject* ) {}

    TileState BombState::Enter(Tile* tile)
    {
        tile->m_BombSpriteComponent->Start();
        tile->m_BombSpriteComponent->SetColumn(static_cast<int>(tile->m_BombType));
        tile->m_PowerUpSpriteComponent->Stop();
        tile->m_ExplosionTime = tile->EXPLOSION_DELAY;
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->AddCollidingChannel(CollisionChannel::Bomb);
        tile->m_CollisionComponent->Start();

        return TileState::Bomb;
    }

    void BombState::Update(Tile* tile, double deltaTime)
    {
        tile->m_ExplosionTime -= deltaTime;
        if (tile->m_ExplosionTime <= 0)
        {
            tile->ChangeState(new ExplosionState());
        }
    }

    void BombState::OnCollision(Tile* , GameObject* , GameObject* ) {}

    TileState PowerUpState::Enter(Tile* tile)
    {
        tile->m_Type = Tile::TileType::Empty;
        tile->m_SpriteComponent->SetColumn(static_cast<int>(tile->m_Type));
        tile->m_BombSpriteComponent->Stop();
        tile->m_PowerUpSpriteComponent->Start();
        tile->m_PowerUpSpriteComponent->SetColumn(static_cast<int>(tile->m_PowerUpType));
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->AddCollidingChannel(CollisionChannel::Default);
        tile->m_CollisionComponent->Start();

        return TileState::PowerUp;
    }

    void PowerUpState::Update(Tile* , double ) {}

    void PowerUpState::OnCollision(Tile* , GameObject* , GameObject* ) {}

    TileState ExplosionState::Enter(Tile* tile)
    {
        tile->m_BombSpriteComponent->Start();
        tile->m_BombSpriteComponent->SetColumn(static_cast<int>(tile->m_BombType));
        tile->m_PowerUpSpriteComponent->Stop();
        tile->m_ExplosionTime = tile->EXPLOSION_TIME;
        tile->m_CollisionComponent->ClearAllChannels();
        tile->m_CollisionComponent->AddCollidingChannel(CollisionChannel::Explosion);
        tile->m_CollisionComponent->Start();

        return TileState::Explosion;
    }

    void ExplosionState::Update(Tile* tile, double deltaTime)
    {
        tile->m_ExplosionTime -= deltaTime;
        if (tile->m_ExplosionTime <= 0)
        {
            tile->ChangeState(new EmptyState());
        }
    }

    void ExplosionState::OnCollision(Tile* , GameObject* , GameObject* ) {}
}

