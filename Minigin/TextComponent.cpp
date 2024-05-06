#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "GameObject.h"

dae::TextComponent::TextComponent(GameObject* m_GameObjectPtr, const std::string& text, std::shared_ptr<Font> font, SDL_Color color )
	: BaseComponent(m_GameObjectPtr)
	, m_needsUpdate(true)
	, m_text(text)
	, m_font(std::move(font))
	, m_textTexture(nullptr)
	, m_color(color)
{
	UpdateTexture();
	m_SpriteComponent = new SpriteComponent(m_GameObjectPtr, m_textTexture);
	m_GameObjectPtr->AddComponent(std::unique_ptr<SpriteComponent>(m_SpriteComponent));
}

void dae::TextComponent::UpdateTexture()
{

	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;

		if (m_SpriteComponent != nullptr)
		{
			m_SpriteComponent->SetTexture(m_textTexture);
		}
	}
}

void dae::TextComponent::Update(double)
{
	UpdateTexture();
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		glm::vec3 pos{};
		if (const TransformComponent* transform = GetGameObject()->GetTransform())
		{
			pos += transform->GetPosition();
		}
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}


