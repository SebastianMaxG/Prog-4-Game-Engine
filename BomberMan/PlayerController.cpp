#include "PlayerController.h"

#include <encdec.h>
#include <iostream>
#include <SDL.h>

#include "GlobalEngineSignals.h"
#include "GlobalSignals.h"
#include "InputHandler.h"
#include "Player.h"

namespace lsmf
{

    PlayerController::PlayerController(GameObject* gameObject, Player* player, int joystickId, bool keyboardInput)
        : ControllerComponent(gameObject)
		, m_ControllerID(joystickId)
		, m_UseKeyboard(keyboardInput)
		, m_Player(player)
    {

        InitCommands();
    }

    PlayerController::PlayerController(GameObject* gameObject, Enemy* player, int joystickId, bool keyboardInput)

        : ControllerComponent(gameObject)
        , m_ControllerID(joystickId)
        , m_UseKeyboard(keyboardInput)
        , m_Player(nullptr)
		, m_Enemy(player)
    {
        InitCommands();
    }

    PlayerController::~PlayerController()
    {
        InputHandler::GetInstance().UnBindCommand("PlayerMove");
        InputHandler::GetInstance().UnBindCommand("Action1");
        InputHandler::GetInstance().UnBindCommand("Action2");
        InputHandler::GetInstance().UnBindCommand("NextLevel");

    }

    void PlayerController::HandleInput(SDL_Event )
	{

    //    if (event.type == SDL_KEYDOWN && m_ControllerID == -1)
    //    {
    //        switch (event.key.keysym.sym)
    //        {
    //        case SDLK_SPACE:
    //            m_Player->PlaceBomb();
    //            break;
    //        case SDLK_LSHIFT:
    //            m_Player->Detonate();
				//break;
    //        default: 
    //            break;
    //        }
    //        return;
    //    }
    //    if (event.type == SDL_CONTROLLERBUTTONDOWN && event.cbutton.which == m_ControllerID)
    //    {
    //        switch (event.cbutton.button)
    //        {
    //        case SDL_CONTROLLER_BUTTON_A:
    //            m_Player->PlaceBomb();
    //            break;
    //        case SDL_CONTROLLER_BUTTON_B:
    //            m_Player->Detonate();
    //            break;
    //        }
    //    }

    // //   auto pos = m_MoveTransform.GetPosition();
    //	//if (m_ControllerID >= 0) 
    // //   {
    // //       if (event.type == SDL_CONTROLLERAXISMOTION && event.caxis.which == m_ControllerID)
    // //       {
	   // //        if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
    // //           {
	   // //        	// Handle horizontal joystick movement
    // //               if (event.caxis.value < -8000)
    // //                {
	   // //                 m_MoveTransform.SetPosition({ -1, pos.y, pos.z });
    // //                	return;
    // //                }
    // //               if (event.caxis.value > 8000)
    // //               {
    // //                   m_MoveTransform.SetPosition({ 1, pos.y, pos.z });
    // //                   return;
    // //               }
    // //           }
	   // //        if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
    // //           {
	   // //        	// Handle vertical joystick movement
    // //               if (event.caxis.value < -8000 )
    // //               {
	   // //                m_MoveTransform.SetPosition({ pos.y, -1, pos.z });
    // //               	return;
    // //               }
    // //               if (event.caxis.value > 8000)
    // //               {
    // //                   m_MoveTransform.SetPosition({ pos.y, 1, pos.z });
    // //                   return;
    // //               }
    // //           }
    // //           m_MoveTransform.SetPosition(0, 0, 0);
    // //            
    // //       }
    // //   }
    // //   else
    // //   {
    // //       if (event.type == SDL_KEYDOWN)
    // //       {
    // //           switch (event.key.keysym.sym)
    // //           {
    // //           case SDLK_UP:
    // //               m_MoveTransform.SetPosition({ 0, -1, 0 });
    // //               m_LastKey = SDLK_UP;
    // //           	break;
    // //           case SDLK_DOWN:
    // //               m_MoveTransform.SetPosition({ 0, 1, 0 });
    // //               m_LastKey = SDLK_DOWN;
    // //           	break;
    // //           case SDLK_LEFT:
    // //               m_MoveTransform.SetPosition({ -1, 0, 0 });
    // //               m_LastKey = SDLK_LEFT;
    // //           	break;
    // //           case SDLK_RIGHT:
    // //               m_MoveTransform.SetPosition({ 1, 0, 0 });
    // //               m_LastKey = SDLK_RIGHT;
    // //           	break;
    // //           }
    // //       }
    // //       else if (event.type == SDL_KEYUP)
    // //       {
    // //           if (event.key.keysym.sym == m_LastKey)
    // //           {
    // //           	m_MoveTransform.SetPosition(0, 0, 0);
    // //           }
    // //       }
    // //   }
    }

    void PlayerController::PlayerMove(SDL_Event event)
    {
        if (m_UseKeyboard)
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    yDir = -1;
                    break;
                case SDLK_DOWN:
                    yDir = 1;
                    break;
                case SDLK_LEFT:
                    xDir = -1;
                    break;
                case SDLK_RIGHT:
                    xDir = 1;
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                case SDLK_DOWN:
                    yDir = 0;
                    break;
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    xDir = 0;
                    break;
                default:
                    break;
                }
            }
        }


        if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
        {
            if (m_ControllerID == event.cdevice.which)
            {
                if (event.type == SDL_CONTROLLERBUTTONDOWN)
                {
                    switch (event.cbutton.button)
                    {
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        yDir = -1;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        yDir = 1;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        xDir = -1;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        xDir = 1;
                        break;
                    default:
                        break;
                    }
                }
                else if (event.type == SDL_CONTROLLERBUTTONUP)
                {
                    switch (event.cbutton.button)
                    {
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        yDir = 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        xDir = 0;
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        m_MoveTransform.SetPosition({ xDir, yDir, 0 });

        if (m_Enemy)
        {
            if (m_MoveTransform.GetPosition().x > 0)
            {
                m_Left = false;
            }
            else if (m_MoveTransform.GetPosition().x < 0)
            {
                m_Left = true;
            }
        }
    }

    void PlayerController::Action1(SDL_Event event)
    {
        
        if (m_UseKeyboard)
        {
            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    m_Player->PlaceBomb();
                }
            }
        }
        if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if (m_ControllerID == event.cdevice.which)
            {
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    m_Player->PlaceBomb();
                }
            }
        }
    }

    void PlayerController::Action2(SDL_Event event)
    {
        if (m_UseKeyboard)
        {
            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_LSHIFT)
                {
                    m_Player->PlaceBomb();
                }
            }
        }
    	if (event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if (m_ControllerID == event.cdevice.which)
            {
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
                {
                    m_Player->PlaceBomb();
                }
            }
        }
    }

    void PlayerController::NextLevel(SDL_Event event)
    {
        if (event.type == SDL_KEYDOWN or !m_UseKeyboard)
        {
            return;
        }
        if (event.key.keysym.sym == SDLK_F2)
        {
            globalSignals::OnPlayerWin.Emit();
            globalSignals::OnPlayerWin.Update();
        }
        else
        {
	        std::cout << "Next level command not found" << std::endl;
        }

        
    }

    void PlayerController::InitCommands()
    {

        {
            auto playerMoveCommand = InputHandler::GetInstance().GetCommand("PlayerMove");
            if (!playerMoveCommand)
            {
                auto playerMove = std::make_unique<Command>();
                playerMoveCommand = playerMove.get();
                playerMoveCommand->BindKey(SDLK_UP);
                playerMoveCommand->BindKey(SDL_CONTROLLER_BUTTON_DPAD_UP);
                playerMoveCommand->BindKey(SDLK_DOWN);
                playerMoveCommand->BindKey(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
                playerMoveCommand->BindKey(SDLK_LEFT);
                playerMoveCommand->BindKey(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
                playerMoveCommand->BindKey(SDLK_RIGHT);
                playerMoveCommand->BindKey(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

                InputHandler::GetInstance().BindCommand("PlayerMove", std::move(playerMove));
            }
            playerMoveCommand->BindFunction(this, &PlayerController::PlayerMove);
        }

        {
            auto action2Command = InputHandler::GetInstance().GetCommand("Action2");
            if (!action2Command)
            {
                auto action2 = std::make_unique<Command>();
                action2Command = action2.get();
                action2Command->BindKey(SDLK_LSHIFT);
                action2Command->BindKey(SDL_CONTROLLER_BUTTON_X);

                InputHandler::GetInstance().BindCommand("Action2", std::move(action2));
            }
            action2Command->BindFunction(this, &PlayerController::Action2);
        }

        {
            auto action1Command = InputHandler::GetInstance().GetCommand("Action1");
            if (!action1Command)
            {
                auto action1 = std::make_unique<Command>();
                action1Command = action1.get();
                action1Command->BindKey(SDLK_SPACE);
                action1Command->BindKey(SDL_CONTROLLER_BUTTON_A);

                InputHandler::GetInstance().BindCommand("Action1", std::move(action1));
            }
            action1Command->BindFunction(this, &PlayerController::Action1);
        }

        {
            auto NextLevelCommand = InputHandler::GetInstance().GetCommand("NextLevel");
            if (!NextLevelCommand)
            {
                auto nextLevel = std::make_unique<Command>();
                NextLevelCommand = nextLevel.get();
                NextLevelCommand->BindKey(SDLK_F2);

                InputHandler::GetInstance().BindCommand("NextLevel", std::move(nextLevel));
            }
            NextLevelCommand->BindFunction(this, &PlayerController::NextLevel);
        }
    }

    void PlayerController::Update(double)
    {
        //SDL_GameController* controller = SDL_GameControllerOpen(m_ControllerID);
        //if (controller)
        //{
        //    int16_t x_move = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        //    int16_t y_move = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

        //    // Apply a deadzone to the joystick movement
        //    if (std::abs(x_move) < JOYSTICK_DEADZONE) x_move = 0;
        //    if (std::abs(y_move) < JOYSTICK_DEADZONE) y_move = 0;

        //    m_MoveTransform.SetPosition({ x_move / 32767.0f, y_move / 32767.0f, 0 });
        //}
        //else
        //{
        //    const Uint8* state = SDL_GetKeyboardState(NULL);
        //    if (state[SDL_SCANCODE_UP]) {
        //        m_MoveTransform.SetPosition({ 0, -1, 0 });
        //    }
        //    else if (state[SDL_SCANCODE_DOWN]) {
        //        m_MoveTransform.SetPosition({ 0, 1, 0 });
        //    }
        //    else if (state[SDL_SCANCODE_LEFT]) {
        //        m_MoveTransform.SetPosition({ -1, 0, 0 });
        //    }
        //    else if (state[SDL_SCANCODE_RIGHT]) {
        //        m_MoveTransform.SetPosition({ 1, 0, 0 });
        //    }
        //    else {
        //        m_MoveTransform.SetPosition({ 0, 0, 0 });
        //    }
        //}
    }

    bool PlayerController::IsMoving()
    {
        if (!m_Player)
        {
            return false;
		}


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
