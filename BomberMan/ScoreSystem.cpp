#include "ScoreSystem.h"

#include "GameObject.h"
#include "GlobalSignals.h"
#include "ResourceManager.h"
#include "TextComponent.h"

namespace lsmf
{
	void ScoreSystem::AddScore(uint32_t score)
	{
		m_Score += score;
		// set score text
		m_TextComponent->SetText("Score: " + std::to_string(m_Score));
	}
	void ScoreSystem::OnEnemyDeathScore(Enemy::EnemyType type)
	{
		switch (type)
		{
		case Enemy::EnemyType::Balloom:
			AddScore(100);
			break;
		case Enemy::EnemyType::Oneal:
			AddScore(200);
			break;
		case Enemy::EnemyType::Doll:
			AddScore(400);
			break;
		case Enemy::EnemyType::Minvo:
			AddScore(800);
			break;
		}
	}
	uint32_t ScoreSystem::GetScore() const
	{
		return m_Score;
	}
	ScoreSystem::ScoreSystem(GameObject* gameObject)
		: BaseComponent(gameObject)
		, m_Score(0)
	{
		m_EnemyConnection = globalSignals::OnEnemyDeath.Connect(this, &ScoreSystem::OnEnemyDeathScore);
		auto font = lsmf::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

		auto textComponent = std::make_unique<TextComponent>(gameObject, "Score: 0", font);
		m_TextComponent = textComponent.get();
		gameObject->AddComponent(std::move(textComponent));
	}
	ScoreSystem::~ScoreSystem() noexcept
	{
	}
}

