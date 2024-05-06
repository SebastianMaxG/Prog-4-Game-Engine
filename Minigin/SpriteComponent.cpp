#include "SpriteComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::SpriteComponent::SpriteComponent(GameObject* m_GameObjectPtr, const std::string& filename)
	: BaseComponent(m_GameObjectPtr)
{
	SetTexture(filename);
}

dae::SpriteComponent::SpriteComponent(GameObject* m_GameObjectPtr, std::shared_ptr<Texture2D> newTexture)
	: BaseComponent(m_GameObjectPtr)
{
	SetTexture(std::move(newTexture));
}

void dae::SpriteComponent::Render() const
{
	glm::vec3 pos{};
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

void dae::SpriteComponent::SetTexture(std::shared_ptr<Texture2D> newTexture)
{
		m_texture = std::move(newTexture);
}
