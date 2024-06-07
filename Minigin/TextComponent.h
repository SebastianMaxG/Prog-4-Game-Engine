#pragma once
#include <string>
#include <memory>
#include "BaseComponent.h"

namespace lsmf
{
	class Font;
	class Texture2D;
	class SpriteComponent;
	class TextComponent final: public lsmf::BaseComponent
	{
	public:
		void Update(double deltaTime) override;
		void Render() const override;

		void SetText(const std::string& text);

		TextComponent(GameObject* m_GameObjectPtr, const std::string& text, std::shared_ptr<Font> font, SDL_Color color = { 255,255,255,255 });
		~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
		SDL_Color m_color;

		SpriteComponent* m_SpriteComponent;
		void UpdateTexture();

	};
}
