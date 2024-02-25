#include "VisualComponent.h"

dae::VisualComponent::VisualComponent(GameObject* m_GameObjectPtr, Transform transform)
	: BaseComponent(m_GameObjectPtr)
	, m_Transform(transform)
{
}

[[maybe_unused]] void dae::VisualComponent::Update(double deltaTime)
{
	(void)(deltaTime);
}
