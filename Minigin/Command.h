#pragma once
#include <glm/vec2.hpp>

#include "GameObject.h"
#include "Signal.h"
#include <SDL.h>

namespace lsmf
{

    class Command final
    {
    public:
		Command() = default;
		~Command() = default;
		void Execute(SDL_Event event) const;

        // static functions only
        bool BindFunction(std::function<void(SDL_Event)> function)
        {
			const auto result = m_BoundFunctions.insert(std::make_unique<signal::StaticFunctionSlot<SDL_Event>>(function));
			return result.second;
        }
        // member functions only
        template<typename ClassType>
        bool BindFunction(ClassType* object, void(ClassType::* function)(SDL_Event))
		{
            const auto result = m_BoundFunctions.insert(std::make_unique<signal::MemberFunctionSlot<ClassType, SDL_Event>>(object, function));
            return result.second;
		}

        bool BindKey(SDL_Keycode key)
        {
            const auto result = m_InputKeys.insert(key);
			return result.second;
		}
    private:
        std::set< std::unique_ptr<signal::BaseSlot<SDL_Event>>> m_BoundFunctions;
        std::set<SDL_Keycode> m_InputKeys;
        
    };


    class BaseCommand
    {
    public:
        BaseCommand(const BaseCommand&) = delete;
        BaseCommand(BaseCommand&&) noexcept = delete;
        BaseCommand& operator=(const BaseCommand&) = delete;
        BaseCommand& operator=(BaseCommand&&) noexcept = delete;

        BaseCommand() = delete;
        BaseCommand(GameObject* pGameObject);

        virtual ~BaseCommand() = default;
        virtual void Execute() = 0;

    protected:
        GameObject* m_pGameObject = nullptr;
    };
    using CommandPtr = std::shared_ptr<BaseCommand>;

    class MoveCommand final : public BaseCommand
    {
    public:
		MoveCommand(GameObject* pGameObject, float x, float y);
        MoveCommand(GameObject* pGameObject, const glm::vec2& direction);
        void Execute() override;
    private:
        glm::vec2 m_Direction;

    };

    class EventCommand final : public BaseCommand
    {
    public:
		EventCommand(GameObject* pGameObject, const std::string& event);
        void Execute() override;

    };

}
