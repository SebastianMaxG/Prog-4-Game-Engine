#pragma once
#include <glm/vec2.hpp>

#include "GameObject.h"

namespace lsmf
{

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
