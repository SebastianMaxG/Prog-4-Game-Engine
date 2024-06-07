#include "Enemy.h"

#include "GameObject.h"
#include "SpriteComponent.h"

namespace dae
{
	Enemy::Enemy(GameObject* gameObject, EnemyType type)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		// create sprite component based on type
		// TODO: set sprite file name
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));
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
	void Enemy::Update(double)
	{
	}
}