#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SpriteComponent;
}

namespace dae
{

	class Tile final : public BaseComponent
	{
	public:
		enum class TileType
		{
			Empty,
			Wall,
			Barrel,
			Crate,
			Bomb,
			Explosion,
			ExplosionUp,
			ExplosionRight,
			Exit
		};

		Tile(GameObject* gameObject, TileType type = TileType::Empty);

		void Update(double deltaTime) override;

	private:
		TileType m_Type;
		SpriteComponent* m_SpriteComponent;

		// TODO: collision component
	};


}
