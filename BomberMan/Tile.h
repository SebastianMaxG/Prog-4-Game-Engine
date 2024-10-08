#pragma once
#include "BaseComponent.h"
#include "Signal.h"

namespace lsmf
{
	class Enemy;
	class Player;
	class TileGrid;
	class CollisionComponent;
	class SpriteComponent;


	class Tile final : public BaseComponent
	{
	public:
		enum class TileType
		{
			Exit = 0,
			Wall = 1,
			Empty = 2,
			Crate = 3
		};
		enum class TileState
		{
			Exit ,
			Wall ,
			Empty ,
			Crate ,
			Bomb ,
			PowerUp,
			Explosion
		};
		enum class BombType
		{
			Bomb = 0,
			Explosion = 1,
			ExplosionHLeftEnd = 2,
			ExplosionHCenter = 3,
			ExplosionHRightEnd = 4,
			ExplosionVTopEnd = 7,
			ExplosionVCenter = 6,
			ExplosionVBotEnd = 5
		};
		enum class PowerUpType
		{
			Bomb = 0,
			Range = 1,
			Speed = 2,
			Remote = 3,
			WallPass = 4,
			BombPass = 5,
			FlamePass = 6,
			Invincible = 7,
			None = 8
		};
		enum class BombDir
		{
			up,
			right,
			left,
			down,
			center,
		};

		Tile(GameObject* gameObject, TileType type = TileType::Empty, bool isExit = false, PowerUpType power = PowerUpType::None);
		~Tile() override;

		void Update(double deltaTime) override;
		void SetGridPos(int x, int y, TileGrid* grid)
		{
			m_GridX = x;
			m_GridY = y;
			m_Grid = grid;
		}
		void GetGridPos(int& x, int& y) const
		{
			x = m_GridX;
			y = m_GridY;
		}
		
		void BreakCrate();

		void CollisionEvent(GameObject* collider, GameObject* other);
		TileState GetState() const { return m_State; }
		void SetCrate(PowerUpType powerUp = PowerUpType::None, bool isExit = false);

		// 
	private:
		// behaviour functions
		void OnExit();
		void OnCrate(double deltaTime);
		void OnWall();
		void OnEmpty();
		void OnBomb(double deltaTime);
		void OnPowerUp();
		void OnExplosion(double deltaTime);


		// enter functions
		void EnterExit();
		void EnterCrate();
		void EnterWall();
		void EnterEmpty();
		void EnterPowerUp();
	public:
		void EnterBomb(int bombRange);
		void EnterExplosion(int range, BombDir dir = BombDir::center);

	private:
		int m_BombRange = 0;
		BombDir m_BombDir{BombDir::center};
		const double EXPLOSION_DELAY = 3.0;
		const double BOMB_FRAMETIME = 0.5;
		const double EXPLOSION_TIME = 1.0;
		double m_ExplosionTime = 0.0;
		

		TileState m_State{TileState::Empty};
		TileType m_Type;
		BombType m_BombType{BombType::Bomb};
		PowerUpType m_PowerUpType{PowerUpType::None};
		SpriteComponent* m_SpriteComponent;
		SpriteComponent* m_PowerUpSpriteComponent;
		SpriteComponent* m_BombSpriteComponent;
		CollisionComponent* m_CollisionComponent;

		double m_BreakTime{};
		bool m_IsCrateBroken = false;
		bool m_ContainsExit = false;
		Player* m_PlayerOnTile {};
		Enemy* m_EnemyOnTile {};

		TileGrid* m_Grid{};
		int m_GridX{};
		int m_GridY{};


		signal::Connection<GameObject*, GameObject*>* m_CollisionConnection{};
	};


}
