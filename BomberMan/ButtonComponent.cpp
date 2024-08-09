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
        auto textComp = std::make_unique<TextComponent>(gameObject, text, font, color);
        m_TextComponent = textComp.get();
        gameObject->AddComponent(std::move(textComp));

        auto buttonSelect = std::make_unique<Command>();
        buttonSelect->BindFunction(this, &ButtonComponent::ButtonSelect);
        buttonSelect->BindKey(SDLK_RETURN);
        buttonSelect->BindKey(SDLK_SPACE);
        buttonSelect->BindKey(SDL_MOUSEBUTTONDOWN);
        buttonSelect->BindKey(SDL_CONTROLLER_BUTTON_A);
        InputHandler::GetInstance().BindCommand("ButtonSelect", std::move(buttonSelect));
    }

    void ButtonComponent::Update(double)
    {
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
            m_TextComponent->SetText("o" + m_Text + "o");
        }
        else
        {
            m_TextComponent->SetText(m_Text);
        }
    }

    bool ButtonComponent::IsSelected() const
    {
        return m_Selected;
    }
    void ButtonComponent::ButtonSelect(SDL_Event )
    {
		if (m_Selected)
		{
            m_OnClick();
		}
    }
}
