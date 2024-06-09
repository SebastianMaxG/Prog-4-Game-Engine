#include "Enemy.h"

#include "CollisionComponent.h"
#include "CollisionHandeler.h"
#include "EnemyController.h"
#include "GameObject.h"
#include "GlobalSignals.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "TileGrid.h"
#include "TransformComponent.h"

namespace lsmf
{
	Enemy::Enemy(GameObject* gameObject, EnemyType type, TileGrid* grid)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "EnemySprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(4, 12, 3, 0.4);
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3);


		auto controllerComponent = std::make_unique<EnemyController>(gameObject, grid);
		m_ControllerComponent = controllerComponent.get();
		gameObject->AddComponent(std::move(controllerComponent));
		m_ControllerComponent->SetSpeed(m_Speed * SPEED_MULTIPLIER);

		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{ 0,0,0,0 }, false);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionComponent->SetChannel(CollisionChannel::Player, CollisionType::Event);
		m_CollisionComponent->SetChannel(CollisionChannel::Explosion, CollisionType::Event);


		m_CollisionConnection = collision::OnCollide.Connect(this, &Enemy::CollisionEvent);
		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x) - 2,static_cast<int>(size.y) - 2 });

		switch (m_Type)
		{
		case EnemyType::Balloom:
			m_Speed = 2;
			break;
		case EnemyType::Oneal:
			m_Speed = 3;
			m_IsSmart = true;
			break;
		case EnemyType::Doll:
			m_Speed = 3;
			break;
		case EnemyType::Minvo:
			m_Speed = 4;
			m_IsSmart = true;
			break;
		}
		m_ControllerComponent->SetSpeed(m_Speed);

		if (EnemyController* controller = dynamic_cast<EnemyController*>(m_ControllerComponent))
		{
			controller->SetUpdateSpeed(static_cast<float>(m_Speed));
		}
	}
	void Enemy::Update(double deltaTime)
	{
		if (m_Dead)
		{
			m_KillTime -= deltaTime;
			if (m_KillTime <= 0)
			{
				GetGameObject()->MarkForDestruction();
				this->Stop();
			}
			return;
		}
		if (const EnemyController* controller = dynamic_cast<EnemyController*>(m_ControllerComponent))
		{
			SetLeft(controller->GetLeft());
		}



	}
	void Enemy::Kill()
	{
		m_Dead = true;
		if (m_Dead)
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3 + 2);
			m_SpriteComponent->SetFrames(4, 12, 4, 0.4);
			m_SpriteComponent->SetFrame();
			m_KillTime = 4 * 0.4;
			m_ControllerComponent->Stop();
			globalSignals::OnEnemyDeath.Emit(m_Type);
		}
	}
	void Enemy::SetLeft(bool left)
	{
		m_Left = left;
		if (m_Left)
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3);
			m_SpriteComponent->SetFrames(4, 12, 3, 0.2);

		}
		else
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3 + 1);
			m_SpriteComponent->SetFrames(4, 12, 3, 0.2);

		}

	}
	void Enemy::CollisionEvent(GameObject* collider, GameObject* other)
	{
		if (collider == GetGameObject())
		{
			if (Player* player = dynamic_cast<Player*>(other->GetComponent(typeid(Player))))
			{
				player->Kill();
			}
		}
	}
}
