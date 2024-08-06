#pragma once
#include <unordered_map>
#include <string>
#include "Command.h"

namespace lsmf
{
    class InputHandler
    {
    public:
        void BindCommand(const std::string& action, CommandPtr command);
        void HandleInput(const std::string& action);

    private:
        std::unordered_map<std::string, CommandPtr> m_CommandMap;
    };
}
