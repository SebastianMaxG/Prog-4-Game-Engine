#include "Transform.h"

namespace lsmf
{

	void Transform::SetPosition(const float x, const float y, const float z)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}

	void Transform::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
	}

	void Transform::Translate(const glm::vec3& pos)
	{
		m_Position += pos;
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}

	void Transform::Rotate(float angle)
	{
		m_Rotation += angle;
	}

	void Transform::SetRotation(float angle)
	{
		m_Rotation = angle;
	}

	const glm::vec3& Transform::GetScale() const
	{
		return m_Scale;
	}

	float Transform::GetRotation() const
	{
		return m_Rotation;
	}

	Transform Transform::operator-(const Transform& other) const
	{
		Transform result{};
		result.m_Position = m_Position - other.m_Position;
		result.m_Rotation = m_Rotation - other.m_Rotation;
		result.m_Scale = m_Scale / other.m_Scale;
		return result;
	}

	Transform Transform::operator+(const Transform& other) const
	{
		Transform result{};
		result.m_Position = m_Position + other.m_Position;
		result.m_Rotation = m_Rotation + other.m_Rotation;
		result.m_Scale = m_Scale * other.m_Scale;
		return result;
	}

	Transform Transform::operator*(const float& other) const
	{
		Transform result{};
		result.m_Position = m_Position * other;
		result.m_Rotation = m_Rotation * other;
		result.m_Scale = m_Scale * other;
		return result;
	}
}