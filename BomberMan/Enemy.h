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
			Balloom,
			Oneal,
			Doll,
			Minvo
		};

		Enemy(GameObject* gameObject, EnemyType type = EnemyType::Balloom);

		void Update(double deltaTime) override;

	private:
		EnemyType m_Type = EnemyType::Balloom;
		SpriteComponent* m_SpriteComponent;

		TransformComponent* m_TransformComponent;

		double m_Speed = 1.0;

		bool m_IsSmart = false;
		// TODO: collision component
	};


}

