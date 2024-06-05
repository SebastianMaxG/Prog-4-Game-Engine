#pragma once
#include <SDL.h>
#include <vector>
#include <glm/vec3.hpp>

#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
		mutable std::vector<std::tuple<SDL_Texture*, SDL_Rect, float>> m_RenderQueue;
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, float z = 0) const;
		void RenderTexture(const Texture2D& texture, glm::vec3 pos) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float z = 0) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

