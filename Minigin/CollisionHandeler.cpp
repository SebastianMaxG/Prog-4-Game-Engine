#include "CollisionHandeler.h"

#include <ranges>

#include "TransformComponent.h"

void lsmf::CollisionHandler::FixedUpdate()
{
    for (auto& object : m_CollisionQueue)
    {
        const auto& [rect, gameObject, isStatic, channels] = object;
        if (isStatic)
            continue;
        for (auto& otherObject : m_CollisionQueue)
        {
            const auto& [otherRect, otherGameObject, otherIsStatic, otherChannels] = otherObject;
            if (!CanCollide(channels,otherChannels))
                continue;
            if (gameObject != otherGameObject)
            {
                if (CheckCollision(rect, otherRect))
                {
                    for (const auto& [channel, type] : channels)
                    {
                        if (otherChannels.contains(channel))
                        {
                            // Handle collision based on type
                            if (type == CollisionType::Physical)
                            {
								// Calculate the penetration depth in both directions
								const float xPenetration = static_cast<float>((rect.w / 2 + otherRect.w / 2) - std::abs(rect.x - otherRect.x));
								const float yPenetration = static_cast<float>((rect.h / 2 + otherRect.h / 2) - std::abs(rect.y - otherRect.y));

								// Move the object by the smallest penetration depth
								if (xPenetration < yPenetration)
								{
									const float x = (rect.x < otherRect.x) ? -xPenetration : xPenetration;
									gameObject->GetTransform()->Translate(glm::vec3(x, 0, 0));
								}
								else
								{
									const float y = (rect.y < otherRect.y) ? -yPenetration : yPenetration;
									gameObject->GetTransform()->Translate(glm::vec3(0, y, 0));
								}
                            }
                            else if (type == CollisionType::Event)
                            {
                                collision::OnCollide.Emit(gameObject, otherGameObject);
                            }
                        }
                    }
                }
            }
        }
    }
    m_CollisionQueue.clear();
}

void lsmf::CollisionHandler::CalculateCollision(SDL_Rect rect, GameObject* gameObject, bool isStatic, std::map<CollisionChannel, CollisionType> channels)
{
    m_CollisionQueue.emplace_back(rect, gameObject, isStatic, channels);
}

bool lsmf::CollisionHandler::CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) const
{
    return (rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.y + rect1.h > rect2.y);
}

bool lsmf::CollisionHandler::CanCollide(const std::map<CollisionChannel, CollisionType>& channels1, const std::map<CollisionChannel, CollisionType>& channels2) const
{
    for (const auto& channel1 : channels1 | std::views::keys)
    {
        if (channels2.contains(channel1))
        {
            return true;
        }
    }

    return false;
}
