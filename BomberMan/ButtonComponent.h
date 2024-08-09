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

        ButtonComponent(GameObject* gameObject, Callback onClick, const std::string& text, std::shared_ptr<Font>& font, SDL_Color color = { 255,255,255,255 });
        ~ButtonComponent() override = default;

        void Update(double deltaTime) override;

        void SetSelected(bool selected);
        bool IsSelected() const;
        void ButtonSelect(SDL_Event e);

    private:
        TextComponent* m_TextComponent{nullptr};
        Callback m_OnClick;
        bool m_Selected;
        std::string m_Text;
    };
}

