#include <SDL.h>
#include "InputManager.h"

namespace lsmf
{
	void InputManager::OpenControllers()
	{
		int numJoysticks = SDL_NumJoysticks();
		for (int i = 0; i < numJoysticks; i++) {
			if (SDL_IsGameController(i)) {
				SDL_GameController* controller = SDL_GameControllerOpen(i);
				if (controller) {
					m_Controllers.push_back(controller);
				}
			}
		}
	}

	void InputManager::CloseControllers()
	{
		for (SDL_GameController* controller : m_Controllers) {
			SDL_GameControllerClose(controller);
		}
		m_Controllers.clear();
	}

	InputManager::InputManager()
	{
		OpenControllers();
	}

	InputManager::~InputManager() noexcept
	{
		CloseControllers();
	}

	bool InputManager::ProcessInput()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return false;
			}
			if
				(
				e.type == SDL_KEYDOWN || 
				e.type == SDL_KEYUP || 
				e.type == SDL_MOUSEBUTTONDOWN || 
				e.type == SDL_MOUSEBUTTONUP || 
				e.type == SDL_CONTROLLERBUTTONDOWN || 
				e.type == SDL_CONTROLLERBUTTONUP || 
				e.type == SDL_CONTROLLERAXISMOTION
				)
				{
					input::InputEvent.Emit(e);
				}

			//if (e.type == SDL_KEYDOWN) 
			//{
			//	
			//}
			//if (e.type == SDL_MOUSEBUTTONDOWN) 
			//{
			//	
			//}
			//if (e.type == SDL_CONTROLLERBUTTONDOWN) 
			//{
			//	// Handle controller button down event
			//}
			//if (e.type == SDL_CONTROLLERBUTTONUP) 
			//{
			//	// Handle controller button up event
			//}
			//if (e.type == SDL_CONTROLLERAXISMOTION)
			//{
			//	// Handle controller axis motion event
			//	if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX || e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
			//	{
			//		// Handle left stick motion
			//	}
			//	else if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX || e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
			//	{
			//		// Handle right stick motion
			//	}
			//}
			// etc...
		}
		input::InputEvent.Update();

		// get the state of controllers and keyboard
		 /*const Uint8* state = SDL_GetKeyboardState(NULL);
		 for (int i = 0; i < m_Controllers.size(); i++)
		 {
			 for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; j++)
			 {
				 if (SDL_GameControllerGetButton(m_Controllers[i], (SDL_GameControllerButton)j))
				 {
					 input::InputEvent.Emit(j, i);
				 }
			 }
		 }*/

		return true;
	}
}
