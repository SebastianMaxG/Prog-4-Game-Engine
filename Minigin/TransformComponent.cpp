#include "TransformComponent.h"
#include "Transform.h"
#include "GameObject.h"

namespace lsmf
{
	TransformComponent::TransformComponent(GameObject* m_GameObjectPtr)
		:BaseComponent(m_GameObjectPtr)
		, m_LocalTransform()
		, m_WorldTransform()
	{
	}

	void TransformComponent::SetPosition(const glm::vec3& pos)
	{
		m_LocalTransform.SetPosition(pos);
		m_IsDirty = true;
	}

	void TransformComponent::Translate(const glm::vec3& pos)
	{
		m_LocalTransform.Translate(pos);
		m_IsDirty = true;
	}

	void TransformComponent::SetScale(const glm::vec3& scale)
	{
		m_LocalTransform.SetScale(scale);
		m_IsDirty = true;
	}

	void TransformComponent::Rotate(float angle)
	{
		m_LocalTransform.Rotate(angle);
		m_IsDirty = true;
	}

	void TransformComponent::SetRotation(float angle)
	{
		m_LocalTransform.SetRotation(angle);
		m_IsDirty = true;
	}

	const glm::vec3& TransformComponent::GetPosition() const
	{
		return GetWorldTransform().GetPosition();

	}

	const glm::vec3& TransformComponent::GetScale() const
	{
		return m_LocalTransform.GetScale();
	}

	float TransformComponent::GetRotation() const
	{
		return m_LocalTransform.GetRotation();
	}
	const Transform& TransformComponent::GetLocalTransform() const
	{
		return m_LocalTransform;
	}
	const Transform& TransformComponent::GetWorldTransform() const 
	{
		if (m_IsDirty)
		{
			if (m_IsRoot)
			{
				m_WorldTransform = m_LocalTransform;
			}
			else
			{
				m_WorldTransform = GetGameObject()->GetParent()->GetTransform()->GetWorldTransform() + m_LocalTransform;
			}
			m_IsDirty = false;
		}
		return m_WorldTransform;
	}
	void TransformComponent::SetLocalTransform(const Transform& transform)
	{
		m_LocalTransform = transform;
		m_IsDirty = true;
	}
	void TransformComponent::SetWorldTransform(const Transform& transform)
	{
		m_WorldTransform = transform;
		m_IsDirty = true;
	}
	void TransformComponent::SetIsRoot(bool isRoot)
	{
		m_IsRoot = isRoot;
	}
	void TransformComponent::SetDirty()
	{
		m_IsDirty = true;
	}
}