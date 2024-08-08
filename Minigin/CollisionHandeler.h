#pragma once
#include <SDL_rect.h>
#include <tuple>
#include <vector>

#include "GameObject.h"
#include "Singleton.h"
#include "CollisionComponent.h"

namespace lsmf
{
	namespace collision
	{
		inline signal::Signal<GameObject*, GameObject*> OnCollide;

		struct CollisionData
		{
			GameObject* pGameObject;
			SDL_Rect rect;
			bool isStatic;
			std::set<CollisionChannel> channels;
			std::multimap<CollisionChannel, CollisionType> responseChannels;
		};
	}
    class Texture2D;
    class CollisionHandler final : public Singleton<CollisionHandler>
    {
        std::vector<collision::CollisionData> m_CollisionQueue;
    public:
        void FixedUpdate();
        void CalculateCollision(const collision::CollisionData& data);
    private:
        static bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2);
        static bool CanCollide(const std::multimap<CollisionChannel, CollisionType>& responseChannels, const std::set<CollisionChannel>& channels);
        static void ResolveCollision(const SDL_Rect& movingRect, const SDL_Rect& staticRect, lsmf::GameObject* gameObject);
    };
}


