#include <SDL.h>
#include "InputManager.h"


void dae::InputManager::OpenControllers()
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

void dae::InputManager::CloseControllers()
{
	for (SDL_GameController* controller : m_Controllers) {
		SDL_GameControllerClose(controller);
	}
	m_Controllers.clear();
}

dae::InputManager::InputManager()
{
	OpenControllers();
}

dae::InputManager::~InputManager() noexcept
{
	CloseControllers();
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
		{
			return false;
		}

		OnEvent.Emit(e);

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
	OnEvent.Update();
	return true;
}
