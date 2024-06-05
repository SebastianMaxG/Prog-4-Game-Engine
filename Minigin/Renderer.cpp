#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <algorithm>

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

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	//Sort the render queue based on the z value
	std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), [](const std::tuple<SDL_Texture*, SDL_Rect, float>& lhs, const std::tuple<SDL_Texture*, SDL_Rect, float>& rhs)
		{
			return std::get<2>(lhs) < std::get<2>(rhs);
		});

	//Render the render queue
	for (const auto& renderable : m_RenderQueue)
	{
		const auto& [texture, dst, z] = renderable;
		SDL_RenderCopy(m_renderer, texture, nullptr, &dst);
	}
	m_RenderQueue.clear();

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float z) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_Texture* tex = texture.GetSDLTexture();
	m_RenderQueue.emplace_back(std::make_tuple<SDL_Texture*, SDL_Rect, const float >(std::move(tex), std::move(dst), std::move(z)));

	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, glm::vec3 pos) const
{
	RenderTexture(texture, pos.x, pos.y, pos.z);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const float z) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_Texture* tex = texture.GetSDLTexture();
	m_RenderQueue.emplace_back(std::make_tuple<SDL_Texture*, SDL_Rect,const float >(std::move(tex), std::move(dst), std::move(z)));
	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
