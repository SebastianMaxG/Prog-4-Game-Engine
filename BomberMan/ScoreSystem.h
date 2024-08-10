#pragma once
#include "BaseComponent.h"
#include "Enemy.h"
#include "signal.h"
#include "HighscoreSystem.h"

namespace lsmf
{
	class HighScoreSystem;
	class TextComponent;

	class ScoreSystem final : public BaseComponent
	{
		// signal thread
		//static std::jthread g_OnEnemyDeath;

	public:
		// signals
		//static signal::Signal<Enemy::EnemyType> OnEnemyDeath{g_OnEnemyDeath};

		ScoreSystem(GameObject* gameObject);
		~ScoreSystem() override;

		void Update(double) override;


		ScoreSystem(const ScoreSystem& other) = delete;
		ScoreSystem(ScoreSystem&& other) = delete;
		ScoreSystem& operator=(const ScoreSystem& other) = delete;
		ScoreSystem& operator=(ScoreSystem&& other) = delete;

		void AddScore(uint32_t score);
		void OnEnemyDeathScore(Enemy::EnemyType type);
		uint32_t GetScore() const;

	private:
		uint32_t m_Score = 0;
		TextComponent* m_TextComponent;

		signal::Connection<Enemy::EnemyType>* m_EnemyConnection;
	};

}
