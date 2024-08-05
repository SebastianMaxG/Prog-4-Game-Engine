#include "Player.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Tile.h"
#include <algorithm>

#include "CollisionHandeler.h"
#include "SoundSystem.h"
#include "Signal.h"

namespace lsmf
{
   

	Player::Player(GameObject* gameObject, int controllerId)
		: BaseComponent(gameObject)
	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "PlayerSprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(3, 5, 3, 0.2);
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));

		auto controllerComponent = std::make_unique<PlayerController>(gameObject, this, controllerId);
		m_ControllerComponent = controllerComponent.get();
		gameObject->AddComponent(std::move(controllerComponent));
		m_ControllerComponent->SetSpeed(m_Speed * SPEED_MULTIPLIER);

		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{ 0,0,0,0 }, false);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionComponent->SetChannel(CollisionChannel::Default, CollisionType::Event);
		m_CollisionComponent->SetChannel(CollisionChannel::Wall, CollisionType::Physical);
		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Event);
		m_CollisionComponent->SetChannel(CollisionChannel::Bomb, CollisionType::Event);
		m_CollisionComponent->SetChannel(CollisionChannel::Explosion, CollisionType::Event);
		m_CollisionComponent->SetChannel(CollisionChannel::Crate, CollisionType::Physical);
		m_CollisionComponent->SetChannel(CollisionChannel::Enemy, CollisionType::Event);


		m_CollisionConnection = collision::OnCollide.Connect(this, &Player::CollisionEvent);
		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x)-2,static_cast<int>(size.y)-2 });
	}

	void Player::Update(double deltaTime)
	{
		m_IsMoving = m_ControllerComponent->IsMoving();
		if (!m_IsMoving and !m_IsDead)
		{
			m_SpriteComponent->SetFrames(3, 5, 1, 0.2);
		}
		else
		{
			m_SpriteComponent->SetFrames(3, 5, 3, 0.2);
		}
		if (m_InvincibleTime > 0)
		{
			m_InvincibleTime -= deltaTime;
		}

		for (Tile* tile : m_Bombs)
		{
			if (tile->GetState() != Tile::TileState::Bomb)
			{
				BombDetonated(tile);
				return;
			}
		}

		if (m_IsDead)
		{
			Stop();
			m_ControllerComponent->Stop();
		}
	}
	void Player::SetState(PlayerState state)
	{
		m_State = state;
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));
	}
	void Player::Kill()
	{
		if (m_InvincibleTime > 0)
		{
			return;
		}
		m_Lives--;
		sound::PlaySoundSignal.Emit("Whistle.mp3", 5);
		if (m_Lives <= 0)
		{
			m_IsDead = true;
			SetState(PlayerState::Dead);
			m_SpriteComponent->SetFrame();

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
		m_CollisionComponent->RemoveChannel(CollisionChannel::Crate);
	}
	void Player::ActivateFlamePass()
	{
		m_FlamePass = true;
		m_CollisionComponent->RemoveChannel(CollisionChannel::Explosion);
	}
	void Player::ActivateBombPass()
	{
		m_BombPass = true;
		m_CollisionComponent->RemoveChannel(CollisionChannel::Bomb);
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
		if (collider == GetGameObject())
		{
			if (Tile* tile = dynamic_cast<Tile*>(other->GetComponent(typeid(Tile))))
			{
				m_CurrentTile = tile;
			}
		}
	}

	void Player::PlaceBomb()
	{
		if (!m_CurrentTile)
		{
			return;
		}
		if (m_CurrentTile->GetState() != Tile::TileState::Empty)
		{
			return;
		}
		if (m_NrOfBombs >= m_BombCount)
		{
			return;
		}
		m_CurrentTile->EnterBomb(m_BombRange);
		m_Bombs.push_back(m_CurrentTile);
		m_NrOfBombs++;
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
}
