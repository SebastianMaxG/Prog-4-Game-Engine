#include "Tile.h"

#include "CollisionComponent.h"
#include "CollisionHandeler.h"
#include "GameObject.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "TileGrid.h"
#include "TransformComponent.h"

namespace lsmf
{
	Tile::Tile(GameObject* gameObject, TileType type)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "TileSprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(4, 4, 1, 0.4);
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));

		spriteComponent = std::make_unique<SpriteComponent>(gameObject, "BombSprites.png");
		m_BombSpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_BombSpriteComponent->SetFrames(3, 8, 3, 0.4);
		m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
		m_BombSpriteComponent->Stop();

		spriteComponent = std::make_unique<SpriteComponent>(gameObject, "PowerUpSprite.png");
		m_PowerUpSpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_PowerUpSpriteComponent->SetFrames(1, 8, 1, 0);
		m_PowerUpSpriteComponent->SetColumn(static_cast<int>(m_PowerUpType));
		m_PowerUpSpriteComponent->Stop();


		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{0,0,0,0},true);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionConnection = collision::OnCollide.Connect(this, &CollisionEvent);

		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),static_cast<int>(size.y) });

		switch (m_State)
		{
			case TileState::Exit:
			EnterExit();
			break;
		case TileState::Crate:
			EnterCrate();
			break;
		case TileState::Wall:
			EnterWall();
			break;
		case TileState::Empty:
			EnterEmpty();
			break;
		case TileState::Bomb:
			EnterBomb();
			break;
		case TileState::PowerUp:
			EnterPowerUp();
			break;
		case TileState::Explosion:
			EnterExplosion(0);
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
			if (Player* player = dynamic_cast<Player*>(collider->GetComponent(typeid(player))))
			{
				m_PlayerOnTile = player;
			}
		}
	}

	void Tile::OnExit()
	{
		// TODO: check remaining enemies
		// if enemies == 0
		// enter exit (next level)
		// do this on overlap event
	}

	void Tile::OnCrate(double deltaTime)
	{
		if (m_IsCrateBroken)
		{
			m_BreakTime -= deltaTime;
			if (m_BreakTime <= 0)
			{
				m_IsCrateBroken = false;
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
				//m_PlayerOnTile->IncreaseBombCount();
				break;
			case PowerUpType::Range:
				//m_PlayerOnTile->IncreaseBombRange();
				break;
			case PowerUpType::Speed:
				//m_PlayerOnTile->IncreaseSpeed();
				break;
			case PowerUpType::None:
				break;
			case PowerUpType::Remote:
				//m_PlayerOnTile->ActivateRemote();
				break;
			case PowerUpType::WallPass:
				//m_PlayerOnTile->ActivateWallPass();
				break;
			case PowerUpType::FlamePass:
				//m_PlayerOnTile->ActivateFlamePass();
				break;
			case PowerUpType::BombPass:
				//m_PlayerOnTile->ActivateBombPass();
				break;
			case PowerUpType::Invincible:
				//m_PlayerOnTile->ActivateInvincible();
				break;
			}
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
		// on overlap with player
		// damage player
	}


	void Tile::EnterExit()
	{
		m_Type = TileType::Exit;
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Exit;

		m_CollisionComponent->ClearChannels();
		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Event);
		m_CollisionComponent->Start();

		m_CollisionConnection->Resume();
	}
	void Tile::EnterCrate()
	{
		m_Type = TileType::Crate;
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Crate;

		m_CollisionComponent->ClearChannels();
		m_CollisionComponent->SetChannel(CollisionChannel::Wall, CollisionType::Physical);
		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Physical);
		m_CollisionComponent->Start();

		m_CollisionConnection->Pause();

	}
	void Tile::EnterWall()
	{
		m_Type = TileType::Wall;
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Wall;

		// set the collision to be active for walls
		m_CollisionComponent->ClearChannels();
		m_CollisionComponent->SetChannel(CollisionChannel::Wall, CollisionType::Physical);
		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Physical);
		m_CollisionComponent->Start();

		m_CollisionConnection->Pause();

	}
	void Tile::EnterEmpty()
	{
		m_Type = TileType::Empty;
		m_BombSpriteComponent->Stop();
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Empty;

		m_CollisionComponent->Stop();
		m_CollisionConnection->Pause();

	}
	void Tile::EnterBomb()
	{
		m_BombSpriteComponent->Start();
		m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
		m_PowerUpSpriteComponent->Stop();
		m_State = TileState::Bomb;
		m_ExplosionTime = EXPLOSION_TIME;

		m_CollisionComponent->ClearChannels();
		m_CollisionComponent->SetChannel(CollisionChannel::Bomb, CollisionType::Event);
		m_CollisionComponent->Start();
		m_CollisionConnection->Pause();
	}
	void Tile::EnterPowerUp()
	{
		m_PowerUpSpriteComponent->Start();
		m_PowerUpSpriteComponent->SetColumn(static_cast<int>(m_PowerUpType));
		m_BombSpriteComponent->Stop();
		m_State = TileState::PowerUp;

		m_CollisionComponent->ClearChannels();
		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Event);
		m_CollisionComponent->Start();

		m_CollisionConnection->Resume();
	}
	void Tile::EnterExplosion(int range)
	{
		if (m_Type == TileType::Crate)
		{
			BreakCrate();
			return;
		}
		if (m_State == TileState::PowerUp or m_State == TileState::Explosion or m_State == TileState::Wall)
		{
			return;
		}
		m_BombRange = range;
		if (m_State == TileState::Bomb)
		{
			m_BombDir = BombDir::center;
		}
		m_Type = TileType::Empty;
		m_State = TileState::Explosion;

		m_BombSpriteComponent->Start();
		m_PowerUpSpriteComponent->Stop();
		if (m_BombRange > 0)
		{
			m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
			if (m_Grid)
			{
				switch (m_BombDir)
				{
				case BombDir::center:
					m_Grid->GetTile(m_GridX, m_GridY + 1)->EnterExplosion(range - 1);
					m_Grid->GetTile(m_GridX, m_GridY - 1)->EnterExplosion(range - 1);
					m_Grid->GetTile(m_GridX + 1, m_GridY)->EnterExplosion(range - 1);
					m_Grid->GetTile(m_GridX - 1, m_GridY)->EnterExplosion(range - 1);
					m_BombType = BombType::Explosion;
					break;
				case BombDir::up:
					m_Grid->GetTile(m_GridX, m_GridY + 1)->EnterExplosion(range-1);
					m_BombType = BombType::ExplosionVCenter;
					break;
				case BombDir::down:
					m_Grid->GetTile(m_GridX, m_GridY - 1)->EnterExplosion(range - 1);
					m_BombType = BombType::ExplosionVCenter;
					break;
				case BombDir::right:
					m_Grid->GetTile(m_GridX + 1, m_GridY)->EnterExplosion(range - 1);
					m_BombType = BombType::ExplosionHCenter;
					break;
				case BombDir::left:
					m_Grid->GetTile(m_GridX - 1, m_GridY)->EnterExplosion(range - 1);
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
		m_BombSpriteComponent->SetColumn(static_cast<int>(m_BombType));
		m_ExplosionTime = EXPLOSION_TIME;


		m_CollisionComponent->ClearChannels();
		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Event);
		m_CollisionComponent->Start();

		m_CollisionConnection->Resume();

	}
}
