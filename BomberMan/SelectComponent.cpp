#include "SelectComponent.h"

#include "Command.h"
#include "InputHandler.h"
#include "TextComponent.h"

namespace lsmf
{
    SelectComponent::SelectComponent(GameObject* gameObject, std::shared_ptr<Font>& font)
        : BaseComponent(gameObject)
    {
        //make a text component
        auto textComponent = std::make_unique<TextComponent>(gameObject, " A A A ", font, SDL_Color{ 255, 255, 255, 255 });
        m_TextComponent = textComponent.get();
        gameObject->AddComponent(std::move(textComponent));

        //assign the commands to the functions
        auto nextLetter = std::make_unique<Command>();
        nextLetter->BindKey(SDLK_RIGHT);
        nextLetter->BindKey(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        nextLetter->BindFunction(this, &SelectComponent::NextLetter);
        InputHandler::GetInstance().BindCommand("NextLetter", std::move(nextLetter));

        auto previousLetter = std::make_unique<Command>();
        previousLetter->BindKey(SDLK_LEFT);
        previousLetter->BindKey(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        previousLetter->BindFunction(this, &SelectComponent::PreviousLetter);
        InputHandler::GetInstance().BindCommand("PreviousLetter", std::move(previousLetter));

        auto increment = std::make_unique<Command>();
        increment->BindKey(SDLK_UP);
        increment->BindKey(SDL_CONTROLLER_BUTTON_DPAD_UP);
        increment->BindFunction(this, &SelectComponent::Increment);
        InputHandler::GetInstance().BindCommand("Increment", std::move(increment));

        auto decrement = std::make_unique<Command>();
        decrement->BindKey(SDLK_DOWN);
        decrement->BindKey(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        decrement->BindFunction(this, &SelectComponent::Decrement);
        InputHandler::GetInstance().BindCommand("Decrement", std::move(decrement));
    }

    SelectComponent::~SelectComponent()
    {
		InputHandler::GetInstance().RemoveCommand("NextLetter");
		InputHandler::GetInstance().RemoveCommand("PreviousLetter");
		InputHandler::GetInstance().RemoveCommand("Increment");
		InputHandler::GetInstance().RemoveCommand("Decrement");
    }

    void SelectComponent::Update(double )
    {
        if (m_NeedsUpdate)
        {
            m_NeedsUpdate = false;
            m_TextComponent->SetText(GetMarkedString());
        }
    }

    std::string SelectComponent::GetString() const
    {
        // convert the vector of chars to a string
        std::string str{};
        for (char c : m_Letters)
        {
            str += c;
		}
        return str;
    }

    void SelectComponent::NextLetter(SDL_Event e)
    {
        if (e.type == SDL_KEYUP or e.type == SDL_CONTROLLERBUTTONUP)
        {
            ++m_CurrentIndex;
            if (m_CurrentIndex >= m_Letters.size())
            {
                m_CurrentIndex = 0;
            }
            m_NeedsUpdate = true;
        }
    }

    void SelectComponent::PreviousLetter(SDL_Event e)
    {
        if (e.type == SDL_KEYUP or e.type == SDL_CONTROLLERBUTTONUP)
        {
            if (m_CurrentIndex == 0)
            {
                m_CurrentIndex = m_Letters.size() - 1;
            }
            else
            {
                --m_CurrentIndex;
            }
            m_NeedsUpdate = true;
        }
    }

    void SelectComponent::Increment(SDL_Event e)
    {

        if (e.type == SDL_KEYDOWN or e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            ++m_Letters[m_CurrentIndex];
            if (m_Letters[m_CurrentIndex] > m_MaxLetter)
            {
                m_Letters[m_CurrentIndex] = m_MinLetter;
            }
            m_NeedsUpdate = true;
        }
    }

    void SelectComponent::Decrement(SDL_Event e)
    {
        if (e.type == SDL_KEYDOWN or e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            --m_Letters[m_CurrentIndex];
            if (m_Letters[m_CurrentIndex] < m_MinLetter)
            {
                m_Letters[m_CurrentIndex] = m_MaxLetter;
            }
            m_NeedsUpdate = true;
        }
    }
    void SelectComponent::Deactivate() const
    {
        m_TextComponent->Stop();
        InputHandler::GetInstance().RemoveCommand("NextLetter");
        InputHandler::GetInstance().RemoveCommand("PreviousLetter");
        InputHandler::GetInstance().RemoveCommand("Increment");
        InputHandler::GetInstance().RemoveCommand("Decrement");

    }
    std::string SelectComponent::GetMarkedString() const
    {
        //return the string with the current letter marked

        std::string str{};
        for (size_t i = 0; i < m_Letters.size(); ++i)
        {
            if (i == m_CurrentIndex)
            {
                str += "o";
            }
            else
            {
	            				str += " ";
			}
            str += m_Letters[i];
            if (i == m_CurrentIndex)
            {
                str += "o";
            }
            else
            {
                str += " ";
            }
        }
        return str;
    }
}
