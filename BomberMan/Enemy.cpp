#include "Enemy.h"

#include <iostream>

#include "CollisionComponent.h"
#include "CollisionHandeler.h"
#include "EnemyController.h"
#include "GameObject.h"
#include "GlobalSignals.h"
#include "Player.h"
#include "PlayerController.h"
#include "RandomNumber.h"
#include "SpriteComponent.h"
#include "TileGrid.h"
#include "TransformComponent.h"

namespace lsmf
{
	Enemy::Enemy(GameObject* gameObject, EnemyType type, TileGrid* grid)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		auto controllerComponent = std::make_unique<EnemyController>(gameObject, grid);
		m_ControllerComponent = controllerComponent.get();
		gameObject->AddComponent(std::move(controllerComponent));

		Init(gameObject, type);
		
	}

	Enemy::Enemy(GameObject* gameObject, int controllerId, bool keyboardInput, EnemyType type)
		: BaseComponent(gameObject)
		, m_Type(type)
		, m_IsPlayer(true)
	{
		auto controllerComponent = std::make_unique<PlayerController>(gameObject,this, controllerId, keyboardInput);
		m_ControllerComponent = controllerComponent.get();
		gameObject->AddComponent(std::move(controllerComponent));

		Init(gameObject, type);

	}

	Enemy::~Enemy()
	{
		m_CollisionConnection->Disconnect();
	}

	void Enemy::Update(double deltaTime)
	{
		if (m_Dead)
		{
			m_KillTime -= deltaTime;
			if (m_KillTime <= 0)
			{
				if (!m_IsPlayer)
					GetGameObject()->MarkForDestruction();
				this->Stop();
				m_SpriteComponent->Stop();
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
		if (!m_Dead)
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3 + 2);
			m_SpriteComponent->SetFrames(4, 12, 4, 0.4);
			m_SpriteComponent->SetFrame();
			m_KillTime = 1.55;
			m_ControllerComponent->Stop();
			m_CollisionComponent->Stop();
			globalSignals::OnEnemyDeath.Emit(m_Type);
			m_Dead = true;
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

	void Enemy::Reset(TileGrid* tileGrid)
	{
		// reset the enemy and set it to a random position
		m_Dead = false;
		m_KillTime = 0;

		m_ControllerComponent->Start();
		m_CollisionComponent->Start();
		m_SpriteComponent->Start();
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3);
		m_SpriteComponent->SetFrames(4, 12, 3, 0.4);
		m_SpriteComponent->SetFrame();


		auto tiles = tileGrid->GetEmptyTiles();
		if (tiles.empty())
		{
			std::cout << "No empty tiles found" << std::endl;
			return;
		}
		size_t index = lsmf::random::GetRandomNumber(13, tiles.size());
		auto tile = tiles[index];
		auto playerStart = tile->GetGameObject()->GetTransform()->GetPosition();

		GetGameObject()->GetTransform()->SetPosition({ playerStart.x, playerStart.y, 2 });

	}

	void Enemy::Init(GameObject* gameObject, EnemyType type)
	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "EnemySprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(4, 12, 3, 0.4);
		m_SpriteComponent->SetColumn(static_cast<int>(type) * 3);
		m_SpriteComponent->SetZ(1);
		m_SpriteComponent->SetPingPong(true);

		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{ 0,0,0,0 }, false);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionComponent->AddCollidingChannel(CollisionChannel::Enemy);

		m_CollisionComponent->AddResponseChannel(CollisionChannel::Player, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Explosion, CollisionType::Event);

		m_CollisionComponent->AddResponseChannel(CollisionChannel::Crate, CollisionType::Physical);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Wall, CollisionType::Physical);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Bomb, CollisionType::Physical);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Crate, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Wall, CollisionType::Event);
		m_CollisionComponent->AddResponseChannel(CollisionChannel::Bomb, CollisionType::Event);


		m_CollisionConnection = collision::OnCollide.Connect(this, &Enemy::CollisionEvent);
		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x) - 2,static_cast<int>(size.y) - 2 });

		switch (type)
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
		m_ControllerComponent->SetSpeed(m_Speed * SPEED_MULTIPLIER);

		if (EnemyController* controller = dynamic_cast<EnemyController*>(m_ControllerComponent))
		{
			if (type == EnemyType::Doll)
			{
				controller->LockVerticalMovement();
			}
		}
	}
}
