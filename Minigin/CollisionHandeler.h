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
	}
    class Texture2D;
    class CollisionHandler final : public Singleton<CollisionHandler>
    {
        std::vector<std::tuple<SDL_Rect, GameObject*, bool, std::map<CollisionChannel, CollisionType>>> m_CollisionQueue;  // Update the tuple
    public:
        void FixedUpdate();
        void CalculateCollision(SDL_Rect rect, GameObject* gameObject, bool isStatic, std::map<CollisionChannel, CollisionType> channels);  // Update the method
    private:
        bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) const;
        bool CanCollide(const std::map<CollisionChannel, CollisionType>& channels1, const std::map<CollisionChannel, CollisionType>& channels2) const;  // Update the method
    };
}


