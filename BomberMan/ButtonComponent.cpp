#include "ButtonComponent.h"

#include "InputHandler.h"
#include "InputManager.h"

namespace lsmf
{
    ButtonComponent::ButtonComponent(GameObject* gameObject, Callback onClick, const std::string& text, std::shared_ptr<Font>& font, SDL_Color color)
        : BaseComponent(gameObject)
        , m_OnClick(std::move(onClick))
        , m_Selected(false)
        , m_Text(text)
    {
        auto textComp = std::make_unique<TextComponent>(gameObject, "- " + text + " -", font, color);
        m_TextComponent = textComp.get();
        gameObject->AddComponent(std::move(textComp));

        auto BSPtr =InputHandler::GetInstance().GetCommand("ButtonSelect");
        if (!BSPtr)
        {
            auto buttonSelect = std::make_unique<Command>();
            BSPtr = buttonSelect.get();
            BSPtr->BindKey(SDLK_RETURN);
            BSPtr->BindKey(SDLK_SPACE);
            BSPtr->BindKey(SDL_MOUSEBUTTONDOWN);
            BSPtr->BindKey(SDL_CONTROLLER_BUTTON_B);

            InputHandler::GetInstance().BindCommand("ButtonSelect", std::move(buttonSelect));
		}
        BSPtr->BindFunction(this, &ButtonComponent::ButtonSelect);
    }

    ButtonComponent::~ButtonComponent()
    {
		InputHandler::GetInstance().RemoveCommand("ButtonSelect");
    }

    void ButtonComponent::Update(double)
    {
        if (m_Selected && m_Enabled)
        {
            m_OnClick();
            m_Enabled = false;
        }
    }


    void ButtonComponent::SetSelected(bool selected)
    {
        if (m_Selected == selected)
        {
            return;
		}
        m_Selected = selected;

        if (m_Selected)
        {
            m_TextComponent->SetText("o " + m_Text + " o");
        }
        else
        {
            m_TextComponent->SetText("- " + m_Text + " -");
        }
    }

    bool ButtonComponent::IsSelected() const
    {
        return m_Selected;
    }
    void ButtonComponent::ButtonSelect(SDL_Event e)
    {
        if (e.type != SDL_KEYUP and e.type != SDL_CONTROLLERBUTTONUP)
        {
            return;
        }
		if (m_Selected)
		{
            m_Enabled = true;
		}
    }
}
