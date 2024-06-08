#include "Player.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

namespace lsmf
{


	Player::Player(GameObject* gameObject)
		: BaseComponent(gameObject)
	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "PlayerSprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(3, 5, 3, 0.2);
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));

		auto controllerComponent = std::make_unique<PlayerController>(gameObject, this, 0);
		m_ControllerComponent = controllerComponent.get();
		gameObject->AddComponent(std::move(controllerComponent));
		m_ControllerComponent->SetSpeed(100);

		auto collisionComponent = std::make_unique<CollisionComponent>(gameObject, SDL_Rect{ 0,0,0,0 }, false);
		m_CollisionComponent = collisionComponent.get();
		gameObject->AddComponent(std::move(collisionComponent));

		m_CollisionComponent->AddChannel(CollisionChannel::Default, CollisionType::Physical);

		//set the collision component to the size of the sprite
		const glm::vec2 size = m_SpriteComponent->GetTextureSize();
		const glm::vec3 pos = GetGameObject()->GetTransform()->GetWorldTransform().GetPosition();
		m_CollisionComponent->SetHitbox({ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),static_cast<int>(size.y) });
	}

	void Player::Update(double)
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
	}
	void Player::SetState(PlayerState state)
	{
		m_State = state;
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));
	}
}
