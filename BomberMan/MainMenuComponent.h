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
        ~MainMenuComponent() override = default;

        void Update(double deltaTime) override;

        void AddButton(std::shared_ptr<ButtonComponent> button);

    private:
        std::vector<std::shared_ptr<ButtonComponent>> m_Buttons;
        size_t m_CurrentIndex;

        void UpdateSelection() const;
    };
}

