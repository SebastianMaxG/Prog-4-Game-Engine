#pragma once
#include "BaseComponent.h"
#include "Signal.h"

namespace lsmf
{
	class TileGrid;
}

namespace lsmf
{
	class CollisionComponent;
}

namespace lsmf
{
	class ControllerComponent;
}

namespace lsmf
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

		Enemy(GameObject* gameObject, EnemyType type = EnemyType::Balloom, TileGrid* grid = nullptr);

		void Update(double deltaTime) override;

		void Kill();
		void SetLeft(bool left);


		void CollisionEvent(GameObject* collider, GameObject* other);

	private:
		EnemyType m_Type = EnemyType::Balloom;
		SpriteComponent* m_SpriteComponent;

		TransformComponent* m_TransformComponent;
		ControllerComponent* m_ControllerComponent;
		CollisionComponent* m_CollisionComponent;

		double m_Speed = 1.0;

		const double SPEED_MULTIPLIER = 16.0;

		double m_KillTime{};

		bool m_IsSmart = false;
		bool m_Left = false;
		bool m_Dead = false;


		signal::Connection<GameObject*, GameObject*>* m_CollisionConnection{};
	};


}

