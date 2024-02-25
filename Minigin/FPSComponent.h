#pragma once
#include "TextComponent.h"
#include <SDL.h>
namespace dae
{
	class FPSComponent : public dae::TextComponent
	{
	public:
		FPSComponent(GameObject* m_GameObjectPtr, std::shared_ptr<Font> font, Transform transform = Transform(), SDL_Color color = { 255,255,0,255 });
		void Update(double deltaTime) override;
	private:
		double m_FpsTimer ;
		const double m_FpsUpdateDelay;
		int m_FpsCounter;
		double m_Fps;
	};
}
