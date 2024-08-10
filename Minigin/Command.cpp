#include "Command.h"

#include <iostream>

lsmf::BaseCommand::BaseCommand(GameObject* pGameObject)
	: m_pGameObject(pGameObject)
{}

lsmf::MoveCommand::MoveCommand(GameObject* pGameObject, float x, float y)
	: BaseCommand(pGameObject)
	, m_Direction(x, y)
{
}

lsmf::MoveCommand::MoveCommand(GameObject* pGameObject, const glm::vec2& direction)
	: BaseCommand(pGameObject)
	, m_Direction(direction)
{
}

void lsmf::MoveCommand::Execute()
{
}

void lsmf::Command::Execute(SDL_Event event) const
{

    bool isKeyEvent = (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP);
    bool isControllerEvent = (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP);

    if (isKeyEvent)
    {
        if (!m_InputKeys.contains(event.key.keysym.sym))
        {
            return;
        }
    }
    else if (isControllerEvent)
    {
        if (!m_InputKeys.contains(event.cbutton.button))
        {
            return;
        }
    }
    else
    {
        // If the event is neither a key event nor a controller event, ignore it
        return;
    }
	for (const auto& function : m_BoundFunctions)
	{
		function->Invoke(event);
	}
}
