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
        void UnBindCommand(const std::string& action);
        Command* GetCommand(const std::string& action) const;

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
