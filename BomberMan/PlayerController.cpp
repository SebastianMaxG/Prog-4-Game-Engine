#include "PlayerController.h"
#include <SDL.h>

namespace lsmf {
    PlayerController::PlayerController(GameObject* gameObject, int joystickId)
        : ControllerComponent(gameObject), joystickId(joystickId)
	{}

    void PlayerController::HandleInput(SDL_Event event)
	{
        m_MoveTransform.SetPosition({ 0, 0, 0 });
        m_MoveTransform.SetScale({ 1, 1, 1 });
        m_MoveTransform.SetRotation(0);

    	if (joystickId >= 0) 
        {
            if (event.type == SDL_JOYAXISMOTION && event.jaxis.which == joystickId)
            {
	            if (event.jaxis.axis == 0) 
                {
	            	// Handle horizontal joystick movement
	            	m_MoveTransform.Translate({ static_cast<float>(event.jaxis.value / 32767.0), 0, 0 });
	            }
	            else if (event.jaxis.axis == 1) 
                {
	            	// Handle vertical joystick movement
	            	m_MoveTransform.Translate({ 0, static_cast<float>(event.jaxis.value / 32767.0), 0 });
	            }
            }
        }
        else if (event.type == SDL_KEYDOWN) 
        {
            switch (event.key.keysym.sym)
        	{
            case SDLK_UP:
                m_MoveTransform.Translate({ 0, -1, 0 });
                break;
            case SDLK_DOWN:
                m_MoveTransform.Translate({ 0, 1, 0 });
                break;
            case SDLK_LEFT:
                m_MoveTransform.Translate({ -1, 0, 0 });
                break;
            case SDLK_RIGHT:
                m_MoveTransform.Translate({ 1, 0, 0 });
                break;
            }
        }
    }
}
