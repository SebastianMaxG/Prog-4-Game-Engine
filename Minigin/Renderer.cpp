#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <algorithm>
#include <iostream>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void lsmf::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	float scale = 4.0f;  // The scale factor

	// Scale the renderer
	SDL_RenderSetScale(m_renderer, scale, scale);
}

void lsmf::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	//Sort the render queue based on the z value
	std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), [](const std::tuple<SDL_Texture*, SDL_Rect, float, SDL_Rect>& lhs, const std::tuple<SDL_Texture*, SDL_Rect, float, SDL_Rect>& rhs)
		{
			return std::get<2>(lhs) < std::get<2>(rhs);
		});

	//Render the render queue
	for (const auto& renderable : m_RenderQueue)
	{
		const auto& [texture, dst, z, src] = renderable;
		if (texture == nullptr)
		{
			continue;
		}
		SDL_RenderCopy(m_renderer, texture, &src, &dst);
	}
	m_RenderQueue.clear();

	SDL_RenderPresent(m_renderer);
}

void lsmf::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void lsmf::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float z) const
{
	
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_Texture* tex = texture.GetSDLTexture();
	SDL_Rect src
	{
		0,
		0,
		dst.w,
		dst.h
	};
	m_RenderQueue.push_back(std::make_tuple<SDL_Texture*, SDL_Rect, const float , SDL_Rect>(std::move(tex), std::move(dst), std::move(z),std::move(src)));

	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void lsmf::Renderer::RenderTexture(const Texture2D& texture, glm::vec3 pos) const
{
	RenderTexture(texture, pos.x, pos.y, pos.z);
}

void lsmf::Renderer::RenderTexture(const Texture2D& texture, SDL_Rect dst, SDL_Rect src, float z) const
{
	SDL_Texture* tex = texture.GetSDLTexture();
	m_RenderQueue.emplace_back(std::make_tuple<SDL_Texture*, SDL_Rect, const float , SDL_Rect>(std::move(tex), std::move(dst), std::move(z), std::move(src)));
}

void lsmf::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const float z) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_Texture* tex = texture.GetSDLTexture();
	SDL_Rect src
	{
		0,
		0,
		0,
		0
	};
	SDL_QueryTexture(tex, nullptr, nullptr, &src.w, &src.h);

	m_RenderQueue.emplace_back(std::make_tuple<SDL_Texture*, SDL_Rect,const float, SDL_Rect >(std::move(tex), std::move(dst), std::move(z), std::move(src)));
	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* lsmf::Renderer::GetSDLRenderer() const { return m_renderer; }
