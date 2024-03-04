#include "FPSComponent.h"
#include <format>
dae::FPSComponent::FPSComponent(GameObject* m_GameObjectPtr, std::shared_ptr<Font> font, Transform transform, SDL_Color color)
	: TextComponent(m_GameObjectPtr, "666 FPS", font,transform, color)
	, m_Fps(0)
	, m_FpsUpdateDelay(0.5)
	, m_FpsCounter(0)
	, m_FpsTimer(0)
{
}

void dae::FPSComponent::Update(double deltaTime)
{
	//make the fps 1 decimal
	m_FpsTimer += deltaTime;
	m_FpsCounter++;
	if (m_FpsTimer > m_FpsUpdateDelay)
	{
		m_Fps = m_FpsCounter / m_FpsTimer;
		SetText(std::format("{0:.1f} FPS",m_Fps));
		m_FpsTimer = 0;
		m_FpsCounter = 0;
	}
	__super::Update(deltaTime);
	
}
