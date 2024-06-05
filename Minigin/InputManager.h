#pragma once
#include "Singleton.h"
#include "signal.h"

namespace dae
{
	// Add input signals here

	class InputManager final
	{
	public:
		InputManager();
		~InputManager() noexcept;

		signal::Signal<SDL_Event> OnEvent{};

		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();


	private:
		void OpenControllers();
		void CloseControllers();
		std::vector<SDL_GameController*> m_Controllers {};
	};

}
