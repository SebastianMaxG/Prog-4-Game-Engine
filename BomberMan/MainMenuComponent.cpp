#include "MainMenuComponent.h"

#include "InputHandler.h"
#include "InputManager.h"

namespace lsmf
{
    MainMenuComponent::MainMenuComponent(GameObject* gameObject)
        : BaseComponent(gameObject)
        , m_CurrentIndex(0)
    {
        auto menuMoveCommand = std::make_unique<Command>();
        menuMoveCommand->BindFunction(this, &MainMenuComponent::MenuMove);
        menuMoveCommand->BindKey(SDLK_DOWN);
        menuMoveCommand->BindKey(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        menuMoveCommand->BindKey(SDLK_UP);
        menuMoveCommand->BindKey(SDL_CONTROLLER_BUTTON_DPAD_UP);
        //menuMoveCommand->BindKey(SDL_MOUSEMOTION);
        InputHandler::GetInstance().BindCommand("MenuMove", std::move(menuMoveCommand));

    }

    MainMenuComponent::~MainMenuComponent()
    {
        m_Buttons.clear();
        InputHandler::GetInstance().RemoveCommand("MenuMove");
    }

    void MainMenuComponent::Update(double )
    {
    }

    void MainMenuComponent::AddButton(ButtonComponent* button)
    {
        m_Buttons.push_back(button);
        UpdateSelection();
    }

    void MainMenuComponent::MenuMove(SDL_Event e)
    {
        if (e.type == SDL_KEYUP)
        {
            if (e.key.keysym.sym == SDLK_DOWN)
            {
                m_CurrentIndex = (m_CurrentIndex + 1) % m_Buttons.size();
            }
            else if (e.key.keysym.sym == SDLK_UP)
            {
                m_CurrentIndex = (m_CurrentIndex == 0) ? m_Buttons.size() - 1 : m_CurrentIndex - 1;
            }
        }
        else if (e.type == SDL_CONTROLLERBUTTONUP)
        {
            if (e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
            {
                m_CurrentIndex = (m_CurrentIndex + 1) % m_Buttons.size();
            }
            else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
            {
                m_CurrentIndex = (m_CurrentIndex == 0) ? m_Buttons.size() - 1 : m_CurrentIndex - 1;
            }
        }
        UpdateSelection();
    }

    void MainMenuComponent::UpdateSelection() const
    {
        for (size_t i = 0; i < m_Buttons.size(); ++i)
        {
            m_Buttons[i]->SetSelected(i == m_CurrentIndex);
        }
    }
}