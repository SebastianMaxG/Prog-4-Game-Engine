#pragma once
#include <unordered_map>
#include <string>
#include "Command.h"
#include "Singleton.h"

namespace lsmf
{
    class InputHandler : public Singleton<InputHandler>
    {
    public:
        void BindCommand(const std::string& action, CommandPtr command);
        void HandleInput(const std::string& action);

    private:
        friend class Singleton<InputHandler>;
        InputHandler() = default;

        std::unordered_map<std::string, std::unique_ptr<Command>> m_CommandMap;
    };
}
