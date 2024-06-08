#include "Player.h"

#include "GameObject.h"
#include "SpriteComponent.h"

namespace lsmf
{


	Player::Player(GameObject* gameObject)
		: BaseComponent(gameObject)
	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "PlayerSprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(3, 5, 3, 0.4);
		m_SpriteComponent->SetColumn(static_cast<int>(m_State));

	}

	void Player::Update(double)
	{
		if (!m_IsMoving and !m_IsDead)
		{
			m_SpriteComponent->SetFrame(0);
		}
	}
}
