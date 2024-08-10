#include "InputHandler.h"

#include <iostream>
#include <ranges>

namespace lsmf
{
    void InputHandler::BindCommand(const std::string& action, std::unique_ptr<Command> command)
    {
        m_CommandMap[action] = std::move(command);
    }

    void InputHandler::RemoveCommand(const std::string& action)
    {
        if (m_CommandMap.find(action) != m_CommandMap.end())
        {
            m_CommandMap.erase(action);
        }
    }

    Command* InputHandler::GetCommand(const std::string& action) const
    {
        //if a command is not found, return nullptr
        if (const auto it = m_CommandMap.find(action); it != m_CommandMap.end())
        {
            return it->second.get();
        }

        return nullptr;
    }

    bool InputHandler::HandleInput()
    {
		SDL_Event e;
        SDL_GameControllerUpdate();
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                return false;
            }
            for (const auto& command : m_CommandMap | std::views::values)
            {
	            command->Execute(e);
            }
        }
		return true;
    }

    InputHandler::~InputHandler()
    {
        m_CommandMap.clear();
        CloseControllers();
    }

    InputHandler::InputHandler()
    {
        OpenControllers();
    }

    void InputHandler::OpenControllers()
    {
        const int numJoysticks = SDL_NumJoysticks();
        std::cout << "Number of joysticks detected: " << numJoysticks << std::endl;

        for (int i = 0; i < numJoysticks; i++)
        {
            if (SDL_IsGameController(i))
            {
                SDL_GameController* controller = SDL_GameControllerOpen(i);
                if (controller)
                {
                    m_Controllers.push_back(controller);
                    std::cout << "Opened controller " << i << ": " << SDL_GameControllerName(controller) << std::endl;
                }
                else
                {
                    std::cerr << "Could not open gamecontroller " << i << ": " << SDL_GetError() << std::endl;
                }
            }
            else
            {
                std::cout << "Joystick " << i << " is not a game controller." << std::endl;
            }
        }
    }

    void InputHandler::CloseControllers()
    {
        for (SDL_GameController* controller : m_Controllers) 
        {
            SDL_GameControllerClose(controller);
        }
        m_Controllers.clear();
    }
}
