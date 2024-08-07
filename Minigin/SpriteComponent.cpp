#include "SpriteComponent.h"

#include <iostream>

#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "GameObject.h"

lsmf::SpriteComponent::SpriteComponent(GameObject* m_GameObjectPtr, const std::string& filename)
	: BaseComponent(m_GameObjectPtr)
{
	SetTexture(filename);
}

lsmf::SpriteComponent::SpriteComponent(GameObject* m_GameObjectPtr, std::shared_ptr<Texture2D> newTexture)
	: BaseComponent(m_GameObjectPtr)
{
	SetTexture(std::move(newTexture));
}

void lsmf::SpriteComponent::Update(double deltaTime)
{
	if (m_nrFrames == 1)
	{
		return;
	}
	m_currentTime += deltaTime;
	if (m_currentTime >= m_frameTime)
	{
		m_currentTime -= m_frameTime;
		if (m_pingPong)
		{
			if (m_forward)
			{
				++m_currentFrame;
				if (m_currentFrame >= m_nrFrames - 1)
				{
					m_forward = false;
				}
			}
			else
			{
				--m_currentFrame;
				if (m_currentFrame <= 0)
				{
					m_forward = true;
				}
			}
		}
		else
		{
			++m_currentFrame;
			if (m_currentFrame >= m_nrFrames)
			{
				m_currentFrame %= m_nrFrames;
			}
		}
	}
}

void lsmf::SpriteComponent::Render() const
{
	constexpr float epsilon = 1e-6f;

	if (m_texture == nullptr)
	{
		return;
	}
	glm::vec3 pos{};
	glm::vec3 scale{ 1 , 1,1 };
	if (const TransformComponent* transform = GetGameObject()->GetTransform())
	{
		pos = transform->GetPosition();
		scale = transform->GetScale();
		if (std::abs(scale.x) < epsilon or std::abs(scale.y) < epsilon)
		{
			return;
		}
	}
	if (m_columns == 1 and m_rows == 1)
	{
		if (std::abs(scale.x - 1.f) < epsilon && std::abs(scale.y - 1.f) < epsilon)
		{
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, pos.z + m_Z);
			return;
		}
	}

	SDL_Rect src
	{
		m_currentColumn * m_width,
		m_currentFrame * m_height,
		m_width,
		m_height
	};
	SDL_Rect dst
	{
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(static_cast<float>(m_width)  * scale.x),
		static_cast<int>(static_cast<float>(m_height) * scale.y)
	};
	Renderer::GetInstance().RenderTexture(*m_texture, dst, src, pos.z + m_Z);
}

void lsmf::SpriteComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	SDL_QueryTexture(m_texture->GetSDLTexture(), nullptr, nullptr, &m_width, &m_height);
}

void lsmf::SpriteComponent::SetTexture(std::shared_ptr<Texture2D> newTexture)
{
	m_texture = std::move(newTexture);

	//calculate width and height of a single frame
	SDL_QueryTexture(m_texture->GetSDLTexture(), nullptr, nullptr, &m_width, &m_height);
	m_width /= m_columns;
	m_height /= m_rows;
}

void lsmf::SpriteComponent::SetFrames(int rows, int columns, int nrFrames, double frameTime)
{
	m_rows = rows;
	m_nrFrames = nrFrames;
	m_columns = columns;
	m_frameTime = frameTime;

	//calculate width and height of a single frame
	SDL_QueryTexture(m_texture->GetSDLTexture(), nullptr, nullptr, &m_width, &m_height);
	m_width /= m_columns;
	m_height /= m_rows;
}

void lsmf::SpriteComponent::SetPingPong(bool pingPong)
{
	m_pingPong = pingPong;
	m_forward = true;
}

