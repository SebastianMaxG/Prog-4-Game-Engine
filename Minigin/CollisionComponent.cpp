#include "CollisionComponent.h"

#include "CollisionHandeler.h"
#include "TransformComponent.h"

lsmf::CollisionComponent::CollisionComponent(GameObject* m_GameObjectPtr, SDL_Rect hitBox, bool isStatic)
	: BaseComponent(m_GameObjectPtr)
	, m_HitBox(hitBox)
	, m_IsStatic(isStatic)
{

}

void lsmf::CollisionComponent::FixedUpdate(double)
{
	//set the hitbox x and y to the gameobject x and y
	 m_HitBox.x = static_cast<int>(GetGameObject()->GetTransform()->GetPosition().x);
	 m_HitBox.y = static_cast<int>(GetGameObject()->GetTransform()->GetPosition().y);
	CollisionHandler::GetInstance().CalculateCollision(
		{
			GetGameObject(),
			m_HitBox,
			m_IsStatic,
			m_CollidingChannels,
			m_ResponseChannels
		});
}

void lsmf::CollisionComponent::RemoveResponseChannel(CollisionChannel channel, CollisionType type)
{
	m_ResponseChannels.erase(std::remove_if(m_ResponseChannels.begin(), m_ResponseChannels.end(), [channel, type](const auto& c)
	{
					return c.first == channel && c.second == type;
		}), m_ResponseChannels.end());
}
