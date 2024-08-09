#pragma once
#include <map>
#include <set>

#include "BaseComponent.h"

namespace lsmf
{
    enum class CollisionChannel {
        Default,
        Player,
        Enemy,
        Wall,
        Bomb,
        Explosion,
        Crate,
        Exit
    };

    enum class CollisionType {
        Physical,
        Event,
        NoCollision
    };

    class CollisionComponent : public BaseComponent
    {
    public:
        CollisionComponent(GameObject* m_GameObjectPtr, SDL_Rect hitBox, bool isStatic);

        void FixedUpdate(double deltaTime) override;
        void SetHitbox(SDL_Rect hitBox) { m_HitBox = hitBox; }
        void SetStatic(bool isStatic) { m_IsStatic = isStatic; }
        const std::multimap<CollisionChannel, CollisionType>& GetChannels() const { return m_ResponseChannels; }
        void AddResponseChannel(CollisionChannel channel, CollisionType type) { m_ResponseChannels.insert({ channel, type }); }
        void RemoveResponseChannel(CollisionChannel channel) { m_ResponseChannels.erase(channel); }
        void RemoveResponseChannel(CollisionChannel channel, CollisionType type);

        void AddCollidingChannel(CollisionChannel channel) { m_CollidingChannels.insert(channel); }
        void RemoveCollidingChannel(CollisionChannel channel) { m_CollidingChannels.erase(channel); }

        void ClearResponseChannels() { m_ResponseChannels.clear(); }
        void ClearCollidingChannels() { m_CollidingChannels.clear(); }
        void ClearAllChannels() { m_ResponseChannels.clear(); m_CollidingChannels.clear(); }

    private:
        SDL_Rect m_HitBox;
        bool m_IsStatic = true;
        std::multimap<CollisionChannel, CollisionType> m_ResponseChannels;
        std::set<CollisionChannel> m_CollidingChannels;
    };
}
