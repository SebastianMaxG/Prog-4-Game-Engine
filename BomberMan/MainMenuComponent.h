#pragma once
#include <vector>
#include <memory>
#include "BaseComponent.h"
#include "ButtonComponent.h"

namespace lsmf
{
    class MainMenuComponent final : public BaseComponent
    {
    public:
        MainMenuComponent(GameObject* gameObject);
        ~MainMenuComponent() override;

        void Update(double deltaTime) override;

        void AddButton(ButtonComponent* button);

        void MenuMove(SDL_Event e);

    private:
        std::vector<ButtonComponent*> m_Buttons;
        size_t m_CurrentIndex;

        void UpdateSelection() const;
    };
}

