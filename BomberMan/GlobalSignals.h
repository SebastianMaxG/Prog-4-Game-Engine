#pragma once
#include <thread>

#include "Enemy.h"
#include "signal.h"

namespace lsmf
{
	namespace globalSignals
	{
		inline std::jthread g_OnEnemyDeathThread;
		inline signal::Signal<Enemy::EnemyType> OnEnemyDeath{ g_OnEnemyDeathThread };
	}
}
