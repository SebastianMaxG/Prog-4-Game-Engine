#pragma once
#include <memory>
#include <vector>
#include <string>
#include "BaseComponent.h"
#include "TextComponent.h"

namespace lsmf
{
    class SelectComponent final : public BaseComponent
    {
    public:
        enum class Mode
        {
            Buttons,
            Letters
        };

        SelectComponent(GameObject* gameObject, std::shared_ptr<TextComponent> textComponent);
        ~SelectComponent() override = default;

        void Update(double deltaTime) override;
        void Render() const override;

        void SwitchMode();
        void Next();
        void Previous();

    private:
        Mode m_CurrentMode;
        std::shared_ptr<TextComponent> m_TextComponent;
        std::vector<std::string> m_Buttons;
        std::vector<std::string> m_Letters;
        size_t m_CurrentIndex;

        void UpdateText();
    };
}
