#include "CollisionComponent.h"

#include "CollisionHandeler.h"

lsmf::CollisionComponent::CollisionComponent(GameObject* m_GameObjectPtr, SDL_Rect hitBox, bool isStatic)
	: BaseComponent(m_GameObjectPtr)
	, m_HitBox(hitBox)
	, m_IsStatic(isStatic)
{

}

void lsmf::CollisionComponent::FixedUpdate(double)
{
	CollisionHandler::GetInstance().CalculateCollision(m_HitBox, GetGameObject(), m_IsStatic, m_Channels);
}
