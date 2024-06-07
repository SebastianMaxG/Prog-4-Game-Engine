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
			Exit = 1,
			Wall = 2,
			Empty = 3,
			Crate = 4
		};

		Tile(GameObject* gameObject, TileType type = TileType::Empty);

		void Update(double deltaTime) override;

		void BreakCrate();

	private:
		TileType m_Type;
		SpriteComponent* m_SpriteComponent;

		double m_BreakTime{};
		bool m_IsCrateBroken = false;

		// TODO: collision component
	};


}
