#include "ButtonComponent.h"
#include "InputManager.h"

namespace lsmf
{
    ButtonComponent::ButtonComponent(GameObject* gameObject, std::shared_ptr<TextComponent> textComponent, Callback onClick)
        : BaseComponent(gameObject)
        , m_TextComponent(std::move(textComponent))
        , m_OnClick(std::move(onClick))
        , m_Selected(false)
    {
    }

    void ButtonComponent::Update(double)
    {
        // Handle mouse input
        //TODO: Implement input
        //if (/* condition to check if mouse is over the button and clicked */)
        //{
        //    m_OnClick();
        //}

        //// Handle controller input
        //if (m_Selected&& /* condition to check if the controller button is pressed */)
        //{
        //    m_OnClick();
        //}
    }


    void ButtonComponent::SetSelected(bool selected)
    {
        m_Selected = selected;
        // Update the text component to reflect the selection state
        const std::string text = m_TextComponent->GetText();

        if (m_Selected)
        {
            m_TextComponent->SetText(">" + text + "<");
        }
        else
        {
            m_TextComponent->SetText(text.substr(1, text.size() - 2));
        }
    }

    bool ButtonComponent::IsSelected() const
    {
        return m_Selected;
    }
}
