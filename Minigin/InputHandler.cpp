#include "InputHandler.h"

namespace lsmf
{
    void InputHandler::BindCommand(const std::string& action, CommandPtr command)
    {
        m_CommandMap[action] = std::move(command);
    }

    void InputHandler::HandleInput(const std::string& action)
    {
        auto it = m_CommandMap.find(action);
        if (it != m_CommandMap.end())
        {
            it->second->Execute();
        }
    }
}