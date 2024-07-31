#include "SelectComponent.h"
#include "TextComponent.h"

namespace lsmf
{
    SelectComponent::SelectComponent(GameObject* gameObject, std::shared_ptr<TextComponent> textComponent)
        : BaseComponent(gameObject)
        , m_TextComponent(std::move(textComponent))
        , m_CurrentMode(Mode::Buttons)
        , m_CurrentIndex(0)
    {
        // Initialize buttons and letters
        m_Buttons = { "Button1", "Button2", "Button3" };
        m_Letters =
        {
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
            "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
        };

        UpdateText();
    }

    void SelectComponent::Update(double deltaTime)
    {
        // Handle input for switching modes, next, and previous
        // This is just a placeholder, replace with actual input handling
        if (/* condition to switch mode */)
        {
            SwitchMode();
        }
        if (/* condition to go to next item */)
        {
            Next();
        }
        if (/* condition to go to previous item */)
        {
            Previous();
        }
    }

    void SelectComponent::Render() const
    {
        // Render logic if needed
    }

    void SelectComponent::SwitchMode()
    {
        m_CurrentMode = (m_CurrentMode == Mode::Buttons) ? Mode::Letters : Mode::Buttons;
        m_CurrentIndex = 0; // Reset index when switching modes
        UpdateText();
    }

    void SelectComponent::Next()
    {
        if (m_CurrentMode == Mode::Buttons)
        {
            m_CurrentIndex = (m_CurrentIndex + 1) % m_Buttons.size();
        }
        else
        {
            m_CurrentIndex = (m_CurrentIndex + 1) % m_Letters.size();
        }
        UpdateText();
    }

    void SelectComponent::Previous()
    {
        if (m_CurrentMode == Mode::Buttons)
        {
            m_CurrentIndex = (m_CurrentIndex == 0) ? m_Buttons.size() - 1 : m_CurrentIndex - 1;
        }
        else
        {
            m_CurrentIndex = (m_CurrentIndex == 0) ? m_Letters.size() - 1 : m_CurrentIndex - 1;
        }
        UpdateText();
    }

    void SelectComponent::UpdateText()
    {
        if (m_CurrentMode == Mode::Buttons)
        {
            m_TextComponent->SetText(m_Buttons[m_CurrentIndex]);
        }
        else
        {
            m_TextComponent->SetText(m_Letters[m_CurrentIndex]);
        }
    }
}