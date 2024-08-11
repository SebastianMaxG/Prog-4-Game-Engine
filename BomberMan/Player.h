#pragma once
#include <glm/vec2.hpp>

#include "BaseComponent.h"
#include "Signal.h"
#include "TileGrid.h"

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
		Player(GameObject* gameObject, TileGrid* grid, int controllerId = -1, bool keyboardInput = false);
		~Player() override;

		void Update(double deltaTime) override;
		void FixedUpdate(double deltaTime) override;
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

		void Reset(TileGrid* grid);

		void BombDetonated(Tile* tile);

		void CollisionEvent(GameObject* collider, GameObject* other);
		void PlaceBomb();
		void Detonate();

		bool IsAlive() const { return !m_IsDead; }
	private:

		TileGrid* m_Grid;
		Tile* m_CurrentTile = nullptr;
		std::vector<Tile*> m_Bombs;

		uint8_t m_Lives = 4;
		PlayerState m_State = PlayerState::Down;

		bool m_IsDead = false;
		bool m_IsMoving = false;
		bool m_Remote = true;
		bool m_WallPass = false;
		bool m_FlamePass = false;
		bool m_BombPass = false;
		bool m_BombOverlap = false;
		bool m_BombCollisionDirty = false;


		int m_Speed = 3;
		int m_NrOfBombs = 1;
		int m_BombRange = 1;
		int m_BombCount = 2;


		glm::vec2 m_StartPos = { 0,0 };

		const double m_FrameTime{ 0.2f };
		double m_InvincibleTime = 0;
		const double INVINCIBLE_DURATION = 2.0;
		const double SPEED_MULTIPLIER = 16.0;

		const double m_DeathAnimationTime = 0.6; // Duration of the death animation in seconds
		const double m_DeathFrameTime{ 0.2f };
		double m_DeathAnimationTimer = 0.0; // Timer to track the death animation

		SpriteComponent* m_SpriteComponent;

		PlayerController* m_ControllerComponent;

		CollisionComponent* m_CollisionComponent;


		signal::Connection<GameObject*, GameObject*>* m_CollisionConnection{};
	};

}
