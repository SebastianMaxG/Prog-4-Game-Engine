#include "SpriteComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::SpriteComponent::SpriteComponent(GameObject* m_GameObjectPtr, const std::string& filename, Transform transform)
	: VisualComponent(m_GameObjectPtr, transform)
{
	SetTexture(filename);
}

void dae::SpriteComponent::Render() const
{
	auto pos = m_Transform.GetPosition();
	if (const TransformComponent* transform = GetGameObject()->GetTransform())
	{
		pos += transform->GetPosition();
	}
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::SpriteComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
