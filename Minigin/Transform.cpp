#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

void dae::Transform::Translate(const glm::vec3& pos)
{
	m_Position += pos;
}

void dae::Transform::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

void dae::Transform::Rotate(float angle)
{
	m_Rotation += angle;
}

void dae::Transform::SetRotation(float angle)
{
	m_Rotation = angle;
}

const glm::vec3& dae::Transform::GetScale() const
{
	return m_Scale;
}

float dae::Transform::GetRotation() const
{
	return m_Rotation;
}
