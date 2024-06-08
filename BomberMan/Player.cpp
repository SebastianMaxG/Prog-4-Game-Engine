#include "Player.h"

#include "GameObject.h"
#include "PlayerController.h"
#include "SpriteComponent.h"

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
