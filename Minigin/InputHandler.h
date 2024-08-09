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
        void BindCommand(const std::string& action, std::unique_ptr<Command> command);
        bool HandleInput();

        ~InputHandler() override;
    private:
        friend class Singleton<InputHandler>;
        InputHandler();

        std::unordered_map<std::string, std::unique_ptr<Command>> m_CommandMap;


        void OpenControllers();
        void CloseControllers();
        std::vector<SDL_GameController*> m_Controllers{};
    };
}
