#pragma once
#include "BaseComponent.h"

namespace lsmf
{

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
	private:

		uint8_t m_Lives = 3;
		PlayerState m_State = PlayerState::Down;

		bool m_IsDead = false;
		bool m_IsMoving = false;
		bool m_CanPlaceBomb = true;

		int m_Speed = 1;
		int m_NrOfBombs = 1;
		int m_BombRange = 1;
		int m_BombCount = 0;

		SpriteComponent* m_SpriteComponent;

		PlayerController* m_ControllerComponent;

	};

}
