#pragma once
#include <thread>

#include "Enemy.h"
#include "signal.h"

namespace lsmf
{
	namespace globalSignals
	{
		inline signal::Signal<Enemy::EnemyType> OnEnemyDeath{ };

		inline signal::Signal<> OnPlayerDeath{};

		inline signal::Signal<> OnPlayerWin{};
	}
}
