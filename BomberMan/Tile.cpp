#include "Tile.h"

#include "GameObject.h"
#include "SpriteComponent.h"

namespace lsmf
{
	Tile::Tile(GameObject* gameObject, TileType type)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "TileSprite.png");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));

		m_SpriteComponent->SetFrames(4, 4, 1, 0.4);

		m_SpriteComponent->SetColumn(static_cast<int>(m_Type));


	}
	void Tile::Update(double deltaTime)
	{
		if (m_IsCrateBroken)
		{
			m_BreakTime -= deltaTime;
			if (m_BreakTime <= 0)
			{
				m_IsCrateBroken = false;
				m_Type = TileType::Empty;
				m_SpriteComponent->SetColumn(static_cast<int>(m_Type));
			}
			m_SpriteComponent->SetColumn(4);
		}
	}

	void Tile::BreakCrate()
	{
		if (m_Type == TileType::Crate)
		{
			constexpr double frameTime = 0.4;
			m_SpriteComponent->SetFrames(4, 4, 4, frameTime);
			m_SpriteComponent->SetFrame();
			m_IsCrateBroken = true;
			m_BreakTime = 4 * frameTime;
		}
	}
}
