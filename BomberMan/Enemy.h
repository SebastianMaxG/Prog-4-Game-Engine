#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SpriteComponent;


	class Enemy final : public BaseComponent
	{
	public:
		enum class EnemyType
		{
			Balloom = 0,
			Oneal = 1,
			Doll = 2,
			Minvo = 3
		};

		Enemy(GameObject* gameObject, EnemyType type = EnemyType::Balloom);

		void Update(double deltaTime) override;

		void SetDead(bool dead);
		void SetLeft(bool left);

	private:
		EnemyType m_Type = EnemyType::Balloom;
		SpriteComponent* m_SpriteComponent;

		TransformComponent* m_TransformComponent;

		double m_Speed = 1.0;

		bool m_IsSmart = false;
		bool m_Left = false;
		bool m_Dead = false;
		// TODO: collision component
	};


}

