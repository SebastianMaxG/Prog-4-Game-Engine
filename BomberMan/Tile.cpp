#include "Tile.h"

#include "CollisionComponent.h"
#include "CollisionHandeler.h"
#include "Enemy.h"
#include "GameObject.h"
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



		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{0,0,0,0},true);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionConnection = collision::OnCollide.Connect(this, &Tile::CollisionEvent);

		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),static_cast<int>(size.y) });

		switch (m_Type)
		{
		case TileType::Exit:
			EnterExit();
			break;
		case TileType::Crate:
			EnterCrate();
			break;
		case TileType::Wall:
			EnterWall();
			break;
		case TileType::Empty:
			EnterEmpty();
			break;
		}
	}
	void Tile::Update(double deltaTime)
	{
		switch (m_State)
		{
		case TileState::Exit:
			OnExit();
			break;
		case TileState::Crate:
			OnCrate(deltaTime);
			break;
		case TileState::Wall:
			OnWall();
			break;
		case TileState::Empty:
			OnEmpty();
			break;
		case TileState::Bomb:
			OnBomb(deltaTime);
			break;
		case TileState::PowerUp:
			OnPowerUp();
			break;
		case TileState::Explosion:
			OnExplosion(deltaTime);
			break;
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
	}

	void Tile::SetCrate(PowerUpType powerUp, bool isExit)
	{
		m_ContainsExit = isExit;
		m_PowerUpType = powerUp;
		EnterCrate();
	}

	void Tile::OnExit()
	{
		if (m_PlayerOnTile)
		{
			if (m_Grid->EnemiesAlive())
			{
				return;
			}
			sound::PlaySoundSignal.Emit("Bomberman SFX (6).wav", 5);
			// TODO: Load next level

		}
	}

	void Tile::OnCrate(double deltaTime)
	{
		if (m_IsCrateBroken)
		{
			m_BreakTime -= deltaTime;
			if (m_BreakTime <= 0)
			{
				m_IsCrateBroken = false;
				m_SpriteComponent->SetFrame();
				m_SpriteComponent->SetFrames(4, 4, 1, 0);
				if (m_PowerUpType == PowerUpType::None)
				{
					if (m_ContainsExit)
					{
						EnterExit();
					}
					else
					{
						EnterEmpty();
					}
				}
				else
				{
					EnterPowerUp();
				}
			}
		}
	}
	void Tile::OnWall()
	{
		// Be a Wall
	}
	void Tile::OnEmpty()
	{
		// Be Empty
	}
	void Tile::OnBomb(double deltaTime)
	{
		m_ExplosionTime -= deltaTime;
		if (m_ExplosionTime <= 0)
		{
			EnterExplosion(m_BombRange);

		}
	}

	void Tile::OnPowerUp()
	{
		if (m_PlayerOnTile)
		{
			switch (m_PowerUpType)
			{
			case PowerUpType::Bomb:
				m_PlayerOnTile->IncreaseBombCount();
				break;
			case PowerUpType::Range:
				m_PlayerOnTile->IncreaseBombRange();
				break;
			case PowerUpType::Speed:
				m_PlayerOnTile->IncreaseSpeed();
				break;
			case PowerUpType::None:
				break;
			case PowerUpType::Remote:
				m_PlayerOnTile->ActivateRemote();
				break;
			case PowerUpType::WallPass:
				m_PlayerOnTile->ActivateWallPass();
				break;
			case PowerUpType::FlamePass:
				m_PlayerOnTile->ActivateFlamePass();
				break;
			case PowerUpType::BombPass:
				m_PlayerOnTile->ActivateBombPass();
				break;
			case PowerUpType::Invincible:
				m_PlayerOnTile->ActivateInvincible();
				break;
			}
			sound::PlaySoundSignal.Emit("Bomberman SFX (4).wav", 5);

			if (m_ContainsExit)
			{
				EnterExit();
			}
			else
			{
				EnterEmpty();
			}
		}
		
	}

	void Tile::OnExplosion(double deltaTime)
	{
		m_ExplosionTime -= deltaTime;
		if (m_ExplosionTime <= 0)
		{
			EnterEmpty();
		}
		if (m_PlayerOnTile)
		{
			m_PlayerOnTile->Kill();
		}
		if (m_EnemyOnTile)
		{
			m_EnemyOnTile->Kill();
		}

	}


	void Tile::EnterExit()
	{
		m_Type = TileType::Exit;
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Exit;

		m_CollisionComponent->ClearAllChannels();
		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Exit);
		m_CollisionComponent->Start();

		m_CollisionConnection->Resume();
	}
	void Tile::EnterCrate()
	{
		m_Type = TileType::Crate;
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Crate;

		m_CollisionComponent->ClearAllChannels();
		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Crate);
		m_CollisionComponent->Start();

		m_CollisionConnection->Pause();

	}
	void Tile::EnterWall()
	{
		m_Type = TileType::Wall;
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Wall;

		// set the collision to be active for walls
		m_CollisionComponent->ClearAllChannels();

		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Wall);

		m_CollisionComponent->Start();

		m_CollisionConnection->Pause();

	}
	void Tile::EnterEmpty()
	{
		m_Type = TileType::Empty;
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();

		m_CollisionComponent->ClearAllChannels();
		m_CollisionComponent->Stop();

		m_State = TileState::Empty;

		m_CollisionConnection->Pause();

	}
	void Tile::EnterBomb(int bombRange)
	{
		m_BombRange = bombRange;
		m_BombType = BombType::Bomb;
		m_BombSpriteComponent->Start();
		m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Bomb;
		m_ExplosionTime = EXPLOSION_DELAY;

		m_CollisionComponent->ClearAllChannels();
		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Bomb);
		m_CollisionComponent->Start();
		m_CollisionConnection->Pause();
	}
	void Tile::EnterPowerUp()
	{
		m_Type = TileType::Empty;
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));

		m_PowerUpSpriteComponent->Start();
		m_PowerUpSpriteComponent->SetColumn(static_cast<int>(m_PowerUpType));
		m_BombSpriteComponent->Stop();
		m_State = TileState::PowerUp;

		m_CollisionComponent->ClearAllChannels();
		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Default);
		m_CollisionComponent->Start();

		m_CollisionConnection->Resume();
	}
	void Tile::EnterExplosion(int range, BombDir dir)
	{
		m_BombSpriteComponent->SetFrame();
		m_BombDir = dir;
		if (m_Type == TileType::Crate)
		{
			BreakCrate();
			return;
		}
		if (m_State == TileState::PowerUp or m_State == TileState::Wall)
		{
			return;
		}
		if (m_State == TileState::Bomb)
		{
			m_BombDir = BombDir::center;
			m_BombType = BombType::Explosion;
		}
		else if (range >= 0)
			m_BombRange = range;


		if (m_State == TileState::Explosion)
		{
			m_BombDir = BombDir::center;
		}


		

		m_Type = TileType::Empty;
		m_State = TileState::Explosion;

		m_BombSpriteComponent->Start();
		m_PowerUpSpriteComponent->Stop();
		if (m_BombRange > 0)
		{
			if (m_Grid)
			{
				switch (m_BombDir)
				{
				case BombDir::center:
					m_Grid->GetTile(m_GridX, m_GridY + 1)->EnterExplosion(range - 1, BombDir::up);	   
					m_Grid->GetTile(m_GridX, m_GridY - 1)->EnterExplosion(range - 1, BombDir::down);   
					m_Grid->GetTile(m_GridX + 1, m_GridY)->EnterExplosion(range - 1, BombDir::right);  
					m_Grid->GetTile(m_GridX - 1, m_GridY)->EnterExplosion(range - 1, BombDir::left);   
					m_BombType = BombType::Explosion;
					break;
				case BombDir::up:
					m_Grid->GetTile(m_GridX, m_GridY + 1)->EnterExplosion(range - 1, BombDir::up);
					m_BombType = BombType::ExplosionVCenter;
					break;
				case BombDir::down:
					m_Grid->GetTile(m_GridX, m_GridY - 1)->EnterExplosion(range - 1, BombDir::down);
					m_BombType = BombType::ExplosionVCenter;
					break;
				case BombDir::right:
					m_Grid->GetTile(m_GridX + 1, m_GridY)->EnterExplosion(range - 1, BombDir::right);
					m_BombType = BombType::ExplosionHCenter;
					break;
				case BombDir::left:
					m_Grid->GetTile(m_GridX - 1, m_GridY)->EnterExplosion(range - 1, BombDir::left);
					m_BombType = BombType::ExplosionHCenter;
					break;
				}
			}
		}
		else if (m_BombRange == 0)
		{
			switch (m_BombDir)
			{
			case BombDir::center:
				m_BombType = BombType::Explosion;
				break;
			case BombDir::up:
				m_BombType = BombType::ExplosionVTopEnd;
				break;
			case BombDir::down:
				m_BombType = BombType::ExplosionVBotEnd;
				break;
			case BombDir::right:
				m_BombType = BombType::ExplosionHRightEnd;
				break;
			case BombDir::left:
				m_BombType = BombType::ExplosionHLeftEnd;
				break;
			}
		}
		else
		{
			EnterEmpty();
		}
		m_BombSpriteComponent->SetFrame();

		m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
		m_ExplosionTime = EXPLOSION_TIME;

		m_CollisionComponent->Start();

		m_CollisionComponent->ClearAllChannels();
		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Explosion);

		m_CollisionConnection->Resume();

	}
}
