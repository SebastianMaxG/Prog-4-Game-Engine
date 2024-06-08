#pragma once
#include "ControllerComponent.h"
namespace lsmf
{
	class TileGrid;

	class EnemyController : public ControllerComponent
	{
	public:
		EnemyController(GameObject* m_GameObjectPtr);
		~EnemyController() override = default;

		EnemyController(const EnemyController& other) = delete;
		EnemyController(EnemyController&& other) = delete;
		EnemyController& operator=(const EnemyController& other) = delete;
		const EnemyController& operator=(EnemyController&& other) = delete;

	private:
		signal::Connection<GameObject*, GameObject*>* m_CollisionConnection{};

		TileGrid* m_TileGrid;
	};
}
