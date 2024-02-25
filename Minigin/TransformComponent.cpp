#include "TransformComponent.h"
#include "Transform.h"
namespace dae
{
	dae::TransformComponent::TransformComponent(GameObject* m_GameObjectPtr)
		:BaseComponent(m_GameObjectPtr)
	{
		m_Position = { 0,0,0 };
		m_Scale = { 1,1,1 };
		m_Rotation = 0;
	}

	void TransformComponent::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
	
	}

	void TransformComponent::Translate(const glm::vec3& pos)
	{
		m_Position += pos;
	}

	void TransformComponent::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}

	void TransformComponent::Rotate(float angle)
	{
		m_Rotation += angle;
	}

	void TransformComponent::SetRotation(float angle)
	{
		m_Rotation = angle;
	}

	const glm::vec3& TransformComponent::GetPosition() const
	{
		return m_Position;
	}

	const glm::vec3& TransformComponent::GetScale() const
	{
		return m_Scale;
	}

	float TransformComponent::GetRotation() const
	{
		return m_Rotation;
	}
}