#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TransformComponent.h"

dae::TextComponent::TextComponent(GameObject* m_GameObjectPtr, const std::string& text, std::shared_ptr<Font> font, Transform transform, SDL_Color color )
	: VisualComponent(m_GameObjectPtr, transform)
	, m_needsUpdate(true)
	, m_text(text)
	, m_font(std::move(font))
	, m_textTexture(nullptr)
	, m_color(color)
{ }

void dae::TextComponent::Update(double deltaTime)
{
	deltaTime;
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
	}
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		auto pos = m_Transform.GetPosition();
		if (!m_TransformComponent.expired())
		{
			pos += m_TransformComponent.lock()->GetPosition();
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

void dae::TextComponent::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}


