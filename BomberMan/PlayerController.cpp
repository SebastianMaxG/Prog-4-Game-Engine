#include "PlayerController.h"
#include <SDL.h>

#include "Player.h"

namespace lsmf
{

    PlayerController::PlayerController(GameObject* gameObject, Player* player, int joystickId)
        : ControllerComponent(gameObject)
		, joystickId(joystickId)
		, m_Player(player)
	{}

    void PlayerController::HandleInput(SDL_Event)
	{
     //   auto pos = m_MoveTransform.GetPosition();
    	//if (joystickId >= 0) 
     //   {
     //       if (event.type == SDL_CONTROLLERAXISMOTION && event.caxis.which == joystickId)
     //       {
	    //        if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
     //           {
	    //        	// Handle horizontal joystick movement
     //               if (event.caxis.value < -8000)
     //                {
	    //                 m_MoveTransform.SetPosition({ -1, pos.y, pos.z });
     //                	return;
     //                }
     //               if (event.caxis.value > 8000)
     //               {
     //                   m_MoveTransform.SetPosition({ 1, pos.y, pos.z });
     //                   return;
     //               }
     //           }
	    //        if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
     //           {
	    //        	// Handle vertical joystick movement
     //               if (event.caxis.value < -8000 )
     //               {
	    //                m_MoveTransform.SetPosition({ pos.y, -1, pos.z });
     //               	return;
     //               }
     //               if (event.caxis.value > 8000)
     //               {
     //                   m_MoveTransform.SetPosition({ pos.y, 1, pos.z });
     //                   return;
     //               }
     //           }
     //           m_MoveTransform.SetPosition(0, 0, 0);
     //            
     //       }
     //   }
     //   else
     //   {
     //       if (event.type == SDL_KEYDOWN)
     //       {
     //           switch (event.key.keysym.sym)
     //           {
     //           case SDLK_UP:
     //               m_MoveTransform.SetPosition({ 0, -1, 0 });
     //               m_LastKey = SDLK_UP;
     //           	break;
     //           case SDLK_DOWN:
     //               m_MoveTransform.SetPosition({ 0, 1, 0 });
     //               m_LastKey = SDLK_DOWN;
     //           	break;
     //           case SDLK_LEFT:
     //               m_MoveTransform.SetPosition({ -1, 0, 0 });
     //               m_LastKey = SDLK_LEFT;
     //           	break;
     //           case SDLK_RIGHT:
     //               m_MoveTransform.SetPosition({ 1, 0, 0 });
     //               m_LastKey = SDLK_RIGHT;
     //           	break;
     //           }
     //       }
     //       else if (event.type == SDL_KEYUP)
     //       {
     //           if (event.key.keysym.sym == m_LastKey)
     //           {
     //           	m_MoveTransform.SetPosition(0, 0, 0);
     //           }
     //       }
     //   }
    }

    void PlayerController::Update(double)
    {

        SDL_GameController* controller = SDL_GameControllerOpen(joystickId);
        if (controller)
        {
            int16_t x_move = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            int16_t y_move = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

            // Apply a deadzone to the joystick movement
            if (std::abs(x_move) < JOYSTICK_DEADZONE) x_move = 0;
            if (std::abs(y_move) < JOYSTICK_DEADZONE) y_move = 0;

            m_MoveTransform.SetPosition({ x_move / 32767.0f, y_move / 32767.0f, 0 });
        }
        else
        {
            const Uint8* state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_UP]) {
                m_MoveTransform.SetPosition({ 0, -1, 0 });
            }
            else if (state[SDL_SCANCODE_DOWN]) {
                m_MoveTransform.SetPosition({ 0, 1, 0 });
            }
            else if (state[SDL_SCANCODE_LEFT]) {
                m_MoveTransform.SetPosition({ -1, 0, 0 });
            }
            else if (state[SDL_SCANCODE_RIGHT]) {
                m_MoveTransform.SetPosition({ 1, 0, 0 });
            }
            else {
                m_MoveTransform.SetPosition({ 0, 0, 0 });
            }
        }
    }

    bool PlayerController::IsMoving()
    {
        if (m_MoveTransform.GetPosition().x > 0)
        {
            m_Player->SetState(Player::PlayerState::Right);
        }
        else if (m_MoveTransform.GetPosition().x < 0)
        {
            m_Player->SetState(Player::PlayerState::Left);
        }
        else if (m_MoveTransform.GetPosition().y > 0)
        {
            m_Player->SetState(Player::PlayerState::Down);
        }
        else if (m_MoveTransform.GetPosition().y < 0)
        {
            m_Player->SetState(Player::PlayerState::Up);
        }
		bool isMoving = m_MoveTransform.GetPosition() != glm::vec3(0, 0, 0);


        return isMoving;
    }
}
