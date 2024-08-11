#include "Player.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Tile.h"
#include <algorithm>

#include "CollisionHandeler.h"
#include "GlobalSignals.h"
#include "SoundSystem.h"
#include "Signal.h"

namespace lsmf
{
   

	Player::Player(GameObject* gameObject, TileGrid* grid, int controllerId, bool keyboardInput)
		: BaseComponent(gameObject)
		, m_Grid(grid)
	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "PlayerSprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(3, 5, 3, m_FrameTime);
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));
		m_SpriteComponent->SetPingPong(true);

		auto controllerComponent = std::make_unique<PlayerController>(gameObject, this, controllerId, keyboardInput);
		m_ControllerComponent = controllerComponent.get();
		gameObject->AddComponent(std::move(controllerComponent));
		m_ControllerComponent->SetSpeed(m_Speed * SPEED_MULTIPLIER);

		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{ 0,0,0,0 }, false);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Player);

		m_CollisionComponent->AddResponseChannel(CollisionChannel::Explosion, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Default, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Wall, CollisionType::Physical);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Crate, CollisionType::Physical);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Enemy, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Bomb, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Exit, CollisionType::Event);



		m_CollisionConnection = collision::OnCollide.Connect(this, &Player::CollisionEvent);
		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x)-2,static_cast<int>(size.y)-2 });
		m_StartPos.x = pos.x;
		m_StartPos.y = pos.y;
	}

	Player::~Player()
	{
		m_CollisionConnection->Disconnect();
	}

	void Player::Update(double deltaTime)
	{
		if (m_IsDead)
		{
			m_DeathAnimationTimer -= deltaTime;
			if (m_DeathAnimationTimer <= 0.0)
			{
				if (m_Lives <= 0)
				{

					globalSignals::OnPlayerDeath.Emit();
					globalSignals::OnPlayerDeath.Update();
					Stop();
					m_SpriteComponent->Stop();
					m_CollisionComponent->Stop();
					return;
				}
				m_SpriteComponent->SetFrames(3, 5, 3, m_FrameTime);

				// Respawn the player after the death animation
				const glm::vec3 pos = GetGameObject()->GetTransform()->GetPosition();
				GetGameObject()->GetTransform()->SetPosition({ m_StartPos.x, m_StartPos.y, pos.z });
				m_IsDead = false;
				SetState(PlayerState::Down);
				m_SpriteComponent->SetFrame();
				m_InvincibleTime = INVINCIBLE_DURATION;
				m_ControllerComponent->Start();
			}
			return;
		}

		m_IsMoving = m_ControllerComponent->IsMoving();
		if (!m_IsMoving)
		{
			m_SpriteComponent->SetFrames(3, 5, 1, m_FrameTime);
		}
		else
		{
			m_SpriteComponent->SetFrames(3, 5, 3, m_FrameTime);
			// find the tile the player is on
			const glm::vec3 posRaw = GetGameObject()->GetTransform()->GetPosition();
			const glm::vec2 pos = { posRaw.x, posRaw.y };
			int x, y;
			Tile* tile = m_Grid->GetTile(pos, x, y);
			if (m_CurrentTile != tile)
			{
				m_CurrentTile = tile;
				sound::PlaySoundSignal.Emit("Bomberman SFX (1).wav", 5);
			}
		}
		if (m_InvincibleTime > 0)
		{
			m_InvincibleTime -= deltaTime;
		}

		for (Tile* tile : m_Bombs)
		{
			if (tile->GetState() != TileState::Bomb)
			{
				BombDetonated(tile);
				return;
			}
		}

		if (!m_BombOverlap and m_BombCollisionDirty)
		{
			m_BombCollisionDirty = false;
			m_CollisionComponent->AddResponseChannel(CollisionChannel::Bomb, CollisionType::Physical);
		}
		
	}

	void Player::FixedUpdate(double)
	{
		if (m_BombOverlap)
			m_BombOverlap = false;
	}

	void Player::SetState(PlayerState state)
	{
		m_State = state;
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));
	}

	void Player::Kill()
	{
		if (m_InvincibleTime > 0 || m_IsDead)
		{
			return;
		}
		m_Lives--;
		sound::PlaySoundSignal.Emit("Bomberman SFX (5).wav", 5);
		m_IsDead = true;
		SetState(PlayerState::Dead);
		m_SpriteComponent->SetFrame();
		m_ControllerComponent->Stop();
		m_DeathAnimationTimer = m_DeathAnimationTime;
		m_SpriteComponent->SetFrames(3, 5, 3, m_DeathFrameTime);

		if (m_Lives <= 0)
		{

			// remove the dropable powerups
			m_Remote = false;
			m_BombPass = false;
			m_FlamePass = false;
			m_WallPass = false;

		}
		m_InvincibleTime = INVINCIBLE_DURATION;


	}
	void Player::IncreaseBombCount()
	{
		m_BombCount++;
	}

	void Player::IncreaseBombRange()
	{
		m_BombRange++;
	}
	void Player::IncreaseSpeed()
	{
		m_Speed++;
		m_ControllerComponent->SetSpeed(static_cast<double>(m_Speed) * SPEED_MULTIPLIER);
	}
	void Player::ActivateRemote()
	{
		m_Remote = true;
	}
	void Player::ActivateWallPass()
	{
		m_WallPass = true;
		m_CollisionComponent->RemoveResponseChannel(CollisionChannel::Crate);
	}
	void Player::ActivateFlamePass()
	{
		m_FlamePass = true;
		m_CollisionComponent->RemoveResponseChannel(CollisionChannel::Explosion);
	}
	void Player::ActivateBombPass()
	{
		m_BombPass = true;
		m_CollisionComponent->RemoveResponseChannel(CollisionChannel::Bomb);
	}
	void Player::ActivateInvincible()
	{
		m_InvincibleTime = INVINCIBLE_DURATION;
	}

	void Player::BombDetonated(Tile* tile)
	{
		m_NrOfBombs--;
		m_Bombs.erase(std::ranges::remove(m_Bombs, tile).begin(), m_Bombs.end());
	}

	void Player::CollisionEvent(GameObject* collider, GameObject* other)
	{
		if (m_IsDead || m_BombOverlap)
		{
			return;
		}
		if (collider == GetGameObject())
		{
			if (Tile* tile = dynamic_cast<Tile*>(other->GetComponent(typeid(Tile))))
			{
				if (tile->GetState() == TileState::Bomb)
				{
					m_BombOverlap = true;
				}
			}
		}
	}

	void Player::PlaceBomb()
	{
		if (!m_CurrentTile)
		{
			return;
		}
		if (m_CurrentTile->GetState() != TileState::Empty)
		{
			return;
		}
		if (m_NrOfBombs >= m_BombCount)
		{
			return;
		}
		m_CollisionComponent->RemoveResponseChannel(CollisionChannel::Bomb, CollisionType::Physical);
		m_BombOverlap = true;
		m_BombCollisionDirty = true;
		m_CurrentTile->EnterBomb(m_BombRange);
		m_Bombs.push_back(m_CurrentTile);
		m_NrOfBombs++;
		sound::PlaySoundSignal.Emit("Bomberman SFX (3).wav", 5);
	}
	void Player::Detonate()
	{
		if (m_Remote)
		{
			for (Tile* tile : m_Bombs)
			{
				tile->EnterExplosion(-1);
			}
			m_Bombs.clear();
			m_NrOfBombs = 0;
		}
	}

	// make player ready for the next level
	void Player::Reset(TileGrid* grid)
	{
		m_Lives = 4;
		Start();
		m_CollisionComponent->Start();
		m_ControllerComponent->Start();
		m_SpriteComponent->Start();
		m_IsDead = false;
		SetState(PlayerState::Down);
		m_SpriteComponent->SetFrame();

		m_Grid = grid;
		m_CurrentTile = nullptr;
	}
}
