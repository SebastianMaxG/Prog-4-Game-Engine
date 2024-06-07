#include "ScoreSystem.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"

namespace dae
{
	void ScoreSystem::AddScore(uint32_t score)
	{
		m_Score += score;
		// set score text
		m_TextComponent->SetText(std::to_string(m_Score));
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
		//TODO: link function to signal
		m_EnemyConnection = OnEnemyDeath.Connect(this, &ScoreSystem::OnEnemyDeathScore);
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		auto textComponent = std::make_unique<TextComponent>(gameObject, "0", font);
		m_TextComponent = textComponent.get();
		gameObject->AddComponent(std::move(textComponent));
	}
	ScoreSystem::~ScoreSystem() noexcept
	{
		m_EnemyConnection->Disconnect();
	}
}

