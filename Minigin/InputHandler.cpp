#include "InputHandler.h"

#include <ranges>

namespace lsmf
{
    void InputHandler::BindCommand(const std::string& action, std::unique_ptr<Command> command)
    {
        m_CommandMap[action] = std::move(command);
    }

    void InputHandler::UnBindCommand(const std::string& action)
    {
        m_CommandMap.erase(action);
    }

    Command* InputHandler::GetCommand(const std::string& action) const
    {
        return m_CommandMap.at(action).get();
    }

    bool InputHandler::HandleInput()
    {
		SDL_Event e;
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
        for (int i = 0; i < numJoysticks; i++) 
        {
            if (SDL_IsGameController(i)) 
            {
	            if (SDL_GameController* controller = SDL_GameControllerOpen(i)) 
                {
                    m_Controllers.push_back(controller);
                }
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
