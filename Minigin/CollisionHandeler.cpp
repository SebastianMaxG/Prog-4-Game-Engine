#include "CollisionHandeler.h"

#include <ranges>

#include "TransformComponent.h"

void lsmf::CollisionHandler::FixedUpdate()
{
    for (auto& object : m_CollisionQueue)
    {
        if (object.isStatic)
            continue;
        for (auto& otherObject : m_CollisionQueue)
        {
            if (!CanCollide(object.responseChannels, otherObject.channels))
                continue;
            if (object.pGameObject == otherObject.pGameObject)
                continue;
            if (!CheckCollision(object.rect, otherObject.rect))
                continue;
            for (const auto& [channel, type] : object.responseChannels)
            {
                if (!otherObject.channels.contains(channel))
                    continue;
                // Handle collision based on type
                if (type == CollisionType::Physical)
                {
                    ResolveCollision(object.rect, otherObject.rect, object.pGameObject);
                }
                else if (type == CollisionType::Event)
                {
                    collision::OnCollide.Emit(object.pGameObject, otherObject.pGameObject);
                    collision::OnCollide.Update();
                }
            }
        }
    }
    m_CollisionQueue.clear();
}

void lsmf::CollisionHandler::CalculateCollision(const collision::CollisionData& data)
{
    m_CollisionQueue.emplace_back(data);
}

bool lsmf::CollisionHandler::CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    return (rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.y + rect1.h > rect2.y);
}

bool lsmf::CollisionHandler::CanCollide(const std::multimap<CollisionChannel, CollisionType>& responseChannels, const std::set<CollisionChannel>& channels)
{
    return std::ranges::any_of(responseChannels | std::views::keys, [&channels](const auto& channel)
        {
            return channels.contains(channel);
        });
}
void  lsmf::CollisionHandler::ResolveCollision(const SDL_Rect& movingRect, const SDL_Rect& staticRect, lsmf::GameObject* gameObject) {
    const int dx1 = staticRect.x + staticRect.w - movingRect.x;  // penetration depth from left
    const int dx2 = movingRect.x + movingRect.w - staticRect.x;  // penetration depth from right
    const int dy1 = staticRect.y + staticRect.h - movingRect.y;  // penetration depth from top
    const int dy2 = movingRect.y + movingRect.h - staticRect.y;  // penetration depth from bottom

    const int xCorrection = std::min(dx1, dx2);
    const int yCorrection = std::min(dy1, dy2);

    // Translate the object along the axis with the smaller penetration depth
    if (xCorrection < yCorrection) {
        // Correct along the x axis
        if (dx1 < dx2) {
            // Move left
            gameObject->GetTransform()->Translate(glm::vec3(dx1, 0, 0));
            //movingRect.x -= dx1;
        }
        else {
            // Move right
            gameObject->GetTransform()->Translate(glm::vec3(-dx2, 0, 0));
            //movingRect.x += dx2;
        }
    }
    else {
        // Correct along the y axis
        if (dy1 < dy2) {
            // Move up
            gameObject->GetTransform()->Translate(glm::vec3(0, dy1, 0));
            //movingRect.y -= dy1;
        }
        else {
            // Move down
            gameObject->GetTransform()->Translate(glm::vec3(0, -dy2, 0));
            //movingRect.y += dy2;
        }
    }
}
