#pragma once
#include <functional>
#include <string>
#include "BaseComponent.h"
#include "TextComponent.h"

namespace lsmf
{
    class ButtonComponent final : public BaseComponent
    {
    public:
        using Callback = std::function<void()>;

        ButtonComponent(GameObject* gameObject, std::shared_ptr<TextComponent> textComponent, Callback onClick);
        ~ButtonComponent() override = default;

        void Update(double deltaTime) override;

        void SetSelected(bool selected);
        bool IsSelected() const;

    private:
        std::shared_ptr<TextComponent> m_TextComponent;
        Callback m_OnClick;
        bool m_Selected;
    };
}

