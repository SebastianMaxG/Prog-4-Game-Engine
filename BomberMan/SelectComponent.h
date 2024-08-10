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
        SelectComponent(GameObject* gameObject, std::shared_ptr<Font>& font);
        ~SelectComponent() override;

        void Update(double deltaTime) override;

        std::string GetString() const;

        void NextLetter(SDL_Event e);
        void PreviousLetter(SDL_Event e);
        void Increment(SDL_Event e);
        void Decrement(SDL_Event e);

        void Deactivate() const;

    private:
        std::string GetMarkedString() const;

        TextComponent* m_TextComponent;
        std::vector<char> m_Letters{ 'A', 'A','A' };
        const char m_MinLetter = 'A';
        const char m_MaxLetter = 'Z';
    	size_t m_CurrentIndex{};
        bool m_NeedsUpdate;

    };
}
