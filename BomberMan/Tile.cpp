#include "Tile.h"

#include "GameObject.h"
#include "SpriteComponent.h"

namespace dae
{
	Tile::Tile(GameObject* gameObject, TileType type)
		: BaseComponent(gameObject)
		, m_Type(type)

	{
		// create sprite component based on type
		// TODO: set sprite file name
		auto spriteComponent = std::make_unique<SpriteComponent>(gameObject, "");
		m_SpriteComponent = spriteComponent.get();
		gameObject->AddComponent(std::move(spriteComponent));

	}
}
