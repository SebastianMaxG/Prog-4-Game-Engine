#include "Enemy.h"

#include "GameObject.h"
#include "GlobalSignals.h"
#include "SpriteComponent.h"

namespace lsmf
{
	Enemy::Enemy(GameObject* gameObject, EnemyType type)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "EnemySprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
		m_SpriteComponent->SetFrames(4, 12, 3, 0.4);
		m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3 - 1);

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
	}
	void Enemy::Update(double deltaTime)
	{
		if (m_Dead)
		{
			m_KillTime -= deltaTime;
			if (m_KillTime <= 0)
			{
				GetGameObject()->MarkForDestruction();
			}
			return;
		}

		//TODO: move enemy



	}
	void Enemy::Kill()
	{
		m_Dead = true;
		if (m_Dead)
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3 + 1);
			m_SpriteComponent->SetFrames(4, 12, 4, 0.4);
			m_SpriteComponent->SetFrame();
			m_KillTime = 4 * 0.4;

			globalSignals::OnEnemyDeath.Emit(m_Type);
		}
	}
	void Enemy::SetLeft(bool left)
	{
		m_Left = left;
		if (m_Left)
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3 - 1);
		}
		else
		{
			m_SpriteComponent->SetColumn(static_cast<int>(m_Type) * 3);
		}

	}
}