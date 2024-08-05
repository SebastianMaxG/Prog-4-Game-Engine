#include "ControllerComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"


namespace lsmf
{

	ControllerComponent::ControllerComponent(GameObject* gameObject)
		:BaseComponent(gameObject)
	{
		m_Connection = input::InputEvent.Connect(this, &ControllerComponent::HandleInput);
	}

	ControllerComponent::~ControllerComponent()
	{
		m_Connection->Disconnect();
	}

	void ControllerComponent::FixedUpdate(double deltaTime)
	{
		Move(deltaTime);
	}

	void ControllerComponent::Move(double deltaTime)
	{
		const auto transform = GetGameObject()->GetTransform();
		Transform moveTransform{};
		moveTransform.SetPosition(m_MoveTransform.GetPosition() * static_cast<float>(deltaTime * m_Speed));
		transform->SetLocalTransform(transform->GetLocalTransform() + moveTransform);
	}
}
