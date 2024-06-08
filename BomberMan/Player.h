#pragma once
#include "BaseComponent.h"
#include "Signal.h"

namespace lsmf
{
	class Tile;
	class CollisionComponent;

	class SpriteComponent;
	class PlayerController;

	class Player : public BaseComponent
	{
	public:
		enum class PlayerState
		{
			Up = 0,
			Right = 1,
			Down = 2,
			Left = 3,
			Dead = 4
		};
		Player(GameObject* gameObject);

		void Update(double deltaTime) override;
		void SetState(PlayerState state);
		void Kill();
		void IncreaseBombCount();
		void IncreaseBombRange();
		void IncreaseSpeed();
		void ActivateRemote();
		void ActivateWallPass();
		void ActivateFlamePass();
		void ActivateBombPass();
		void ActivateInvincible();

		void BombDetonated(Tile* tile);

		void CollisionEvent(GameObject* collider, GameObject* other);
		void PlaceBomb();
		void Detonate();
	private:

		Tile* m_CurrentTile = nullptr;
		std::vector<Tile*> m_Bombs;

		uint8_t m_Lives = 3;
		PlayerState m_State = PlayerState::Down;

		bool m_IsDead = false;
		bool m_IsMoving = false;
		bool m_CanPlaceBomb = true;
		bool m_Remote = false;
		bool m_WallPass = false;
		bool m_FlamePass = false;
		bool m_BombPass = false;

		int m_Speed = 2;
		int m_NrOfBombs = 0;
		int m_BombRange = 1;
		int m_BombCount = 1;

		double m_InvincibleTime = 0;
		const double INVINCIBLE_DURATION = 2.0;
		const double SPEED_MULTIPLIER = 16.0;

		SpriteComponent* m_SpriteComponent;

		PlayerController* m_ControllerComponent;

		CollisionComponent* m_CollisionComponent;


		signal::Connection<GameObject*, GameObject*>* m_CollisionConnection{};
	};

}
