#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <chrono>
#include <thread>

#include "CollisionHandeler.h"

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

lsmf::Minigin::Minigin(const std::string& dataPath)
	: ms_per_frame()
	, fixed_time_step(0.01)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_windowPtr = SDL_CreateWindow(
		"Bomber Man - Leander Flossie",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		960,
		SDL_WINDOW_OPENGL
	);

	if (m_windowPtr == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_windowPtr);

	ResourceManager::GetInstance().Init(dataPath);



}

lsmf::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_windowPtr);
	m_windowPtr = nullptr;
	SDL_Quit();
}

void lsmf::Minigin::Run(const std::function<void()>& load)
{
	load();
	DEVMODE dm;
	memset(&dm, 0, sizeof(dm));
	dm.dmSize = sizeof(dm);
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) != 0)
	{
		ms_per_frame = 1000 / static_cast<int>(dm.dmDisplayFrequency);
	}
	else
	{
		ms_per_frame = 1000 / max_framerate;
	}

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& collisionHandeler = CollisionHandler::GetInstance();
	auto input = InputManager{};

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	double lag{};
	while (doContinue)
	{
		const auto current = std::chrono::high_resolution_clock::now();
		double deltaTime = std::chrono::duration<double>(current - lastTime).count();
		if (deltaTime > 0.5)
		{
			deltaTime = 0.5;
		}
		lastTime = current;
		lag += deltaTime;

		doContinue = input.ProcessInput();

		while (lag >= fixed_time_step)
		{
			sceneManager.FixedUpdate(fixed_time_step);
			collisionHandeler.FixedUpdate();
			lag -= fixed_time_step;
		}
		sceneManager.Update(deltaTime);
		renderer.Render();

		const auto sleep_time = current + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);
	}
}

SDL_Window* lsmf::Minigin::GetSDLWindow()
{
	return m_windowPtr;
}
