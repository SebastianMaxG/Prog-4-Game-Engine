#include "FPSComponent.h"
#include "GameObject.h"
#include <format>

lsmf::FPSComponent::FPSComponent(GameObject* m_GameObjectPtr, std::shared_ptr<Font> font, SDL_Color color)
	: BaseComponent(m_GameObjectPtr)
	, m_FpsTimer(0)
	, m_FpsUpdateDelay(0.5)
	, m_FpsCounter(0)
	, m_Fps(0)
{
		m_TextComponent = new TextComponent(m_GameObjectPtr, "0 FPS", std::move(font), color);
		m_GameObjectPtr->AddComponent(std::unique_ptr<TextComponent>(m_TextComponent));
}

void lsmf::FPSComponent::Update(double deltaTime)
{
	m_FpsTimer += deltaTime;
	m_FpsCounter++;
	if (m_FpsTimer > m_FpsUpdateDelay)
	{
		m_Fps = m_FpsCounter / m_FpsTimer;
		m_TextComponent->SetText(std::format("{0:.1f} FPS",m_Fps));
		m_FpsTimer = 0;
		m_FpsCounter = 0;
	}
}
