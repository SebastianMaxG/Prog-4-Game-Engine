#pragma once
#include "BaseComponent.h"

namespace lsmf
{
	class SpriteComponent;


	class Bomb final : public BaseComponent
	{
	public:
		enum class BombType
		{
			Bomb = 0,
			Explosion = 1,
			ExplosionHLeftEnd = 2,
			ExplosionHCenter = 3,
			ExplosionHRightEnd = 4,
			ExplosionVTopEnd = 5,
			ExplosionVCenter = 6,
			ExplosionVBotEnd = 7
		};

		Bomb(GameObject* gameObject);

		void Update(double deltaTime) override;

		void Kill();
		void SetLeft(bool left);

	private:
		BombType m_Type = BombType::Bomb;
		SpriteComponent* m_SpriteComponent;

		TransformComponent* m_TransformComponent;

		double m_Speed = 1.0;

		double m_KillTime{};

		bool m_IsSmart = false;
		bool m_Left = false;
		bool m_Dead = false;
		// TODO: collision component
	};


}

