#include "Command.h"

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

void lsmf::Command::Execute(SDL_Event event) const
{
	if (!m_InputKeys.contains(event.key.keysym.sym))
	{
		return;
	}
	for (const auto& function : m_BoundFunctions)
	{
		function->Invoke(event);
	}
}
