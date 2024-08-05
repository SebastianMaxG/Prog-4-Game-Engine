#include "MainMenuComponent.h"
#include "InputManager.h"

namespace lsmf
{
    MainMenuComponent::MainMenuComponent(GameObject* gameObject)
        : BaseComponent(gameObject)
        , m_CurrentIndex(0)
    {
    }

    void MainMenuComponent::Update(double)
    {
        // Handle controller input for navigation
        //todo: Implement input
        //if (/* condition to check if the down button is pressed */)
        //{
        //    m_CurrentIndex = (m_CurrentIndex + 1) % m_Buttons.size();
        //    UpdateSelection();
        //}
        //else if (/* condition to check if the up button is pressed */)
        //{
        //    m_CurrentIndex = (m_CurrentIndex == 0) ? m_Buttons.size() - 1 : m_CurrentIndex - 1;
        //    UpdateSelection();
        //}
    }

    void MainMenuComponent::AddButton(std::shared_ptr<ButtonComponent> button)
    {
        m_Buttons.push_back(std::move(button));
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