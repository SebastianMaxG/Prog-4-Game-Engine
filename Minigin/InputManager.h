#pragma once
#include <set>

#include "Singleton.h"
#include "signal.h"

namespace lsmf
{
	// Add input signals here
	namespace input
	{
		inline signal::Signal<SDL_Event> InputEvent{};
	}

	class InputManager final
	{
	public:
		InputManager();
		~InputManager() noexcept;

		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();


	private:
		void OpenControllers();
		void CloseControllers();
		std::vector<SDL_GameController*> m_Controllers {};
		std::set 
	};

}
