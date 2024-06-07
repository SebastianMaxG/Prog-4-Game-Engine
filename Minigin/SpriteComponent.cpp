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

void dae::SpriteComponent::Update(double deltaTime)
{
	if (m_nrFrames == 1)
	{
		return;
	}
	m_currentTime += deltaTime;
	if (m_currentTime >= m_frameTime)
	{
		m_currentTime -= m_frameTime;
		++m_currentFrame;
		if (m_currentFrame >= m_nrFrames)
		{
			m_currentFrame %= m_nrFrames;
		}
	}
}

void dae::SpriteComponent::Render() const
{
	if (m_texture == nullptr)
	{
		return;
	}
	glm::vec3 pos{};
	if (const TransformComponent* transform = GetGameObject()->GetTransform())
	{
		pos += transform->GetPosition();
	}
	if (m_nrFrames == 1)
	{
		Renderer::GetInstance().RenderTexture(*m_texture, pos);
		return;
	}

	SDL_Rect src
	{
		m_currentFrame % m_columns * m_width,
		m_currentFrame / m_columns * m_height,
		m_width,
		m_height
	};

	Renderer::GetInstance().RenderTexture(*m_texture, pos, src);
}

void dae::SpriteComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::SpriteComponent::SetTexture(std::shared_ptr<Texture2D> newTexture)
{
	m_texture = std::move(newTexture);

	//calculate width and height of a single frame
	SDL_QueryTexture(m_texture->GetSDLTexture(), nullptr, nullptr, &m_width, &m_height);
	m_width /= m_columns;
	m_height /= m_rows;
}

void dae::SpriteComponent::SetFrames(int rows, int nrFrames, double frameTime)
{
	m_rows = rows;
	m_nrFrames = nrFrames;
	m_columns = rows / nrFrames + 1;
	m_frameTime = frameTime;

	//calculate width and height of a single frame
	SDL_QueryTexture(m_texture->GetSDLTexture(), nullptr, nullptr, &m_width, &m_height);
	m_width /= m_columns;
	m_height /= m_rows;
}
