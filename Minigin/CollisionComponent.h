#pragma once
#include <map>

#include "BaseComponent.h"

namespace lsmf
{
	enum class CollisionChannel {
		Default,
		Player,
		Enemy,
		// Add more channels as needed
	};

	enum class CollisionType {
		Physical,
		Event
	};
	class CollisionComponent : public BaseComponent
	{
		

	public:
		CollisionComponent(GameObject* m_GameObjectPtr, SDL_Rect hitBox, bool isStatic);

		void FixedUpdate(double deltaTime) override;
		void SetHitbox(SDL_Rect hitBox) { m_HitBox = hitBox; }
		void SetStatic(bool isStatic) { m_IsStatic = isStatic; }
		const std::map<CollisionChannel, CollisionType>& GetChannels() const { return m_Channels; }
		void AddChannel(CollisionChannel channel, CollisionType type) { m_Channels[channel] = type; }
	private:
		SDL_Rect m_HitBox;
		bool m_IsStatic = true;

		std::map<CollisionChannel, CollisionType> m_Channels;
	};
}
